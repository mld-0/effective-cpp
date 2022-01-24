#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Widget {
};

class Rational {
};

int main()
{
	//	From the official C++ guidelines: (we suggest) (for your own good) do not cast away const

	//	const specifies to the compiler that a particular object should not be modified, and the compiler enforces this constaint
	//	const can be used outside classes where declaring global or namespace scope constants, 
	//	declaring constant objects at the file, function, or block scope
	//	static class members (<which are shared between all instances of the class>)

	//	With a pointer, the pointer itself and/or the value pointed to may be declared const
	//	C++ variable declarations can be deciphered by reading them R->L
	//	i.e: p1 is a pointer to a char that is const
	char greeting[] = "Hello";				//	array of non-const data
	char* p = greeting;						// 	non-const pointer, non-const data
	const char* const p1 = "greeting";		//	const pointer, const data
	const char* p2 = "abc"; 				//	non-const pointer, const data
	char* const p3 = p;						//	const pointer, non-const data

	//	The following function definitions are equivalent, each takes a pointer to a const object
	void f1(const Widget *pw);
	void f2(Widget const *pw);

	//	An STL iterator is analogous to a pointer (and has operators that allow it to be used like one)
	//	Declaring an iterator const is analogous to declaring a pointer const
	//	The const_iterator is analogous to a pointer to const data

	vector<int> vec;
	const vector<int>::iterator iter = vec.begin();		//	const iterator, iterator cannot be modified but data can be
	vector<int>::const_iterator citer = vec.begin();	//	const_iterator, iterator can be modified but data cannot

	//	const string Widget::foo();		//	member function returns a constant value
	//	string Widget::foo() const;		//	member function is const

	//	const and functions: can have a const return value, and/or const arguments. Class member functions can be const, to denote that the function does not modifiy (the outward appearance of) the object <these functions cannot modify non-mutable class variables?>
	
	//	returning a const value can be a way to reduce client errors
	//	for example, by declaring the return value of the following const:
	const Rational operator*(const Rational& lhs, const Rational& rhs);
	//	makes mistakes like the following invalid:
	Rational a, b, c;
	//(a * b) = c;		//	illegal because of const return value of operator*
	//	well designed custom types prevent the client from making such mistakes
	
	//	a const member function may be invoked on a const object. 
	//	This use of const-ness helps document the interface of the object, 
	//	making operators const where possible increases the usefulness of const objects, (and passing objects by const references is crucial for writing efficent code)
	//	member functions that differ only in their const-ness can be overloaded

	//	there are two notions of object const-ness: 
	//		bitwise constness, whether the data of the object is modified
	//		logical constness, whether the object remains unchanged with-regard-to its public interface
	//	both are essential to consider in the design of const class member functions, (but) it is bitwise-constness that the compiler checks for. <non-mutable> object data members (excluding those that are static) cannot be modified by const member functions (the compiler checks for assignment to non-static data members)
	//	const member functions can modify objects pointed to by object member variables through those pointers, which is not consistent with logical constness (the operators are valid so long as the member pointer itself is not changed)

}

//	Avoiding duplication by casting away const-ness
class TextBlock {
public:
	const char& operator[](size_t position) const {
		//	perform bounds checking common to both operator[] functions
		return text[position];
	}
	char& operator[](size_t position) {
		return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
	}
	string text;
};

//	Summary:
//		const is a tool to help compilers detect usage errors, it can be applied to objects at any scope, to function parameters, to function return types, and to member functions.
//		the compiler enforces bitwise constness, but programs should strive for logical constness
//		When const and non-const member functions have identical functions, code duplication can be avoided by defining one in terms of the other, requiring the casting away of const
