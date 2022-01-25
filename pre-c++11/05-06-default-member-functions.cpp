#include <iostream>
#include <vector>
using namespace std;

//	TODO: 2022-01-25T04:06:40AEDT rule of 4.5/4/3/0 (here or elsewhere?)

//	With the implicit default class member functions, the empty class
//class Empty {};
//	is actually equivalent to (assuming each function is used at least once)
class Empty {
public:
	Empty() = default;								//	default constructor
	~Empty() = default;								//	default destructor
	Empty(const Empty& rhs) = default;				//	default copy constructor
	Empty(Empty&& rhs) = default;					//	default move constructor
	Empty& operator=(const Empty& rhs) = default;	//	default copy assignment
	Empty& operator=(Empty&& rhs) = default;		//	default move assignment
};
//	Explicitly disable a default member function by replacing 'default' with 'delete'
//	The default destructor is virtual only if class inherits from parent that also declares virtual destructor
//	default copy assignment/constructor copy each non-static data members of source object to target object
//	Declaring any constructor will disable the default constructor
//	The default copy/move constructor/assignment functions are invalid for classes with either const member variables or reference member variables
//	Given default member functions will not be generated for a class that inherits from a parent that declares its own equivalent function private and never defines them

//	The default copy constructor/assignment functions perform a memberwise copy of each variable in the class (the copy constructors of each member performing the copies). Likewise the default move constructor/assignment functions perform memberwise moves for each variable in the class.


//	Solution (for some reason (presumedly) for an old C++ version (did it used to not support '= delete'?))
//	inherit class from 'Uncopyable':
class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};
class HomeForSale: private Uncopyable {
	//	...
};
//	again asking the question why not simply '= delete' these functions?


//	If a Widget constructor accepts type 'T', that constructor defines an <implicit> conversion <(rule)> from T->Widget, unless that constructor is declared explicit 
//	Ongoing: 2022-01-25T01:03:07AEDT (basic rules on the use of explicit constructors)


//	LINK: https://stackoverflow.com/questions/4172722/what-is-the-rule-of-three
//	A class that manages a resource should manage only that one resource, not multiple resources
//	The rule of three: If you need to explicitly declare either the destructor, copy constructor or copy assignment operator yourself, you probably need to explicitly declare all three of them.
//	The rule of five: Adds the move constructor and move assignment to the rule of three
//	The rule of zero: When none of the special member functions are declared and the defaults are used

int main()
{
	//	The default member functions are generated as needed:
	Empty e1;				//	requires default constructor and destructor
	Empty e2(e1);			//	requires copy constructor
	e2 = e1;				//	requires copy assignment
	Empty e3(move(e1));		//	requires move constructor
	e3 = e2;				//	requires move assignment

	return 0;
}
