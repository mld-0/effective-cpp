#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Generally, an lvalue is a variable whose address can be taken (whereas an rvalue is one whose address cannot be taken).
//	l/r-valueness is independent of type.
class Widget_i {
public:
	Widget_i(Widget_i&& rhs);		//	rhs is an lvalue, even though it has an rvalue reference type
};
//	Taking the address of 'rhs' in the move-constructor would be valid, hence it is an lvalue (all parameters are lvalues, but they arguments with which they are initalized may be lvalues or rvalues). 
//	Perfect forwarding is passing an argument while preserving the lvalue/rvalue-ness of the argument (see item 30).
//	Copies of rvalues are generally moved-constructed, while copies of lvalues are usually copy-constructed



//	A function object is an object supporting an 'operator()' member function
//	Function objects created through lambda expressions are known as closures <(are they? is this a C++ specific meaning?)>

//	Function/Class template: A template that generates functions/classes
//	Template functions/classes: function/class generated from template


//	Declarations:
extern int x;							//	object declaration
class Widget_ii;						//	class declaration
bool func(const Widget_ii& w);			//	function declaration
enum class Color;						//	scoped enum (see item 10)

//	Defintions:
int x;												//	object definition
class Widget_ii {};									//	class definition
bool func(const Widget_ii& w) { return true; }		//	function defintion
enum class Color { Yello, Red, Blue };				//	scoped enum definition (see item 10)
//	A definition is also a declaration. 
//	<(C++ allows for multiple declarations of a given name, but only one definition)>

//	TODO: 2022-02-19T23:30:56AEDT effective-c++, c++14/00-terminology, what can be done with a declaration vs a definition

//	A function signature is the part of the declaration that specifies parameter and return types (the book definition excludes exception specification) (and some definitions exclude return type)

//	Some actions are defined as 'undefined behaviour'. These can manifest as obvious and non-obvious bugs.



int main()
{
	return 0;
}

