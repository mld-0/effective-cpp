#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	TODO: 2022-01-30T02:08:24AEDT effective-c++, 20-pass-by-const-ref, C++ compiler behaviour in optimising arguments passed by value?

//	By default, C++ passes objects to (and from) functions by value
//	When passing by value, function parameters are initalized using the copy-constructor from the actual argument
//	copy constructors can be expensive 
//	it is oftern more efficent to pass an object by const ref (const, since otherwise the ref could be changed)
//	C++ references are (typically) implemented as pointers
//	primative types should (generally) be passed by value instead of by reference
//	(even) small user defined types (should be) canidiates for pass by reference as they may grow in the future

class Student {
public:
	Student(string n) : name(n) {}
	void speak() const { cout << "I am " << name << "\n"; }
private:
	string name;
};

//	Pass by value (declaring (not declaring, but declaring-and-using) both functions would be ambigious)
//bool validateStudent(Student s) {
//	cout << "got student by value\n";
//	s.speak();
//	return true;
//}

//	Pass by const ref
bool validateStudent(const Student& s) {
	s.speak();
	return true;
}

//	TODO: 2022-01-30T02:26:43AEDT effective-c++, item 20, polymorphic behaviour (virtual functions) (when passing by) reference

//	Passing by reference avoid the slicing problem (when a derived object is passed by value as a base object), where all <specialized> derived class information is sliced off by the copy

//	Iterators and function objects <and what other types?> are designed to be passed by value

int main()
{
	Student s1("Wil");
	validateStudent(s1);

	return 0;
}

//	Summary:
//		prefer pass-by-const-ref over pass by value
//		except for built-in types, standard library iterators, <others?>, and function object types

