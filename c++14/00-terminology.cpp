//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	TODO: 2022-03-03T01:13:19AEDT effective-c++, item 00, terminology, if all variables including primatives are objects, then what are 'object-objects' things that are not primatives?

//	Generally, an lvalue is a variable whose address can be taken (whereas an rvalue is one whose address cannot be taken).
//	l/r-valueness is independent of type.
class Widget_i {
public:
	Widget_i(Widget_i&& rhs);		//	rhs is an lvalue, even though it has an rvalue reference type
};
//	Taking the address of 'rhs' in the move-constructor would be valid, hence it is an lvalue (all parameters are lvalues, but they arguments with which they are initalized may be lvalues or rvalues). 
//	Perfect forwarding is passing an argument while preserving the lvalue/rvalue-ness of the argument (see item 30).
//	Copies of rvalues are generally moved-constructed, while copies of lvalues are usually copy-constructed

//	Besides l/r-values, we have:
//	xvalue: 'eXpiring' value, refers to an object near the end of its lifetime (example: the result of calling a function whose return type is an rvalue reference is an xvalue)
//	glvalue: 'generalized' lvalue, either an lvalue or an xvalue.
//	prvalue: 'pure' rvalue, is an rvalue that is not an xvalue (example: the result of calling a function whose return type is not a reference)


//	TODO: 2022-02-27T03:10:46AEDT effective-c++, item 0, terminology, (we have <new/different> variable categories now (rvalues are now prvalues or xvalues)?), (review link (see below))
//	LINK: https://en.cppreference.com/w/cpp/language/value_category


//	A function object is an object supporting an 'operator()' member function
//	Function objects created through lambda expressions are known as closures <(are they? is this a C++ specific meaning?)>

//	Function/Class template: A template that generates functions/classes
//	Template functions/classes: function/class generated from template

//	function-template / template-function are synonmous

//	'object' is a synonym for 'variable'


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

//	id-expression: <(restricted form of primary expression)> <(the name of a variable)>

//	'cv-unqualified' <(means not const or volatile)>


//	shallow copy: memberwise copy of values and pointers (default copy-ctor/assign)
//	deep copy: memberwise copy of values, pointers are duplicated and deep-copied


//	storage class specifiers:
//		auto
//		register		<pre-C++17>
//		static
//		extern
//		thread_local	<C++11>
//		mutable

//	storage duration:
//		automatic: (local variables), storage is allocated for the enclosing block
//		static: (static/extern and global variables), storage is allocated for the entire program runtime
//		thread: (thread_local variables) <C++11>, storage is allocated for lifetime of thread
//		dynamic: (new/malloc), storage is allocated/deallocated upon request

//	linkage: <(refers to same name introduced by declaration in another scope)>
//	LINK: https://en.cppreference.com/w/cpp/language/storage_duration
//	{{{
//		no linkage: (names declared at block scope), visible only in containing scope
//		internal linkage: (static functions/variables, non-extern const/constexpr variables, names declared in unnamed namespace), all scopes in current translation unit
//		external linkage: (unless they are declared in unnamed namespace or attached to named module and not exported: non-static functions, non-static/const variables, extern variables, enums, classes, static data members, nested classes, friend functions declared in class bodies) (also any of declared at block scope: extern variables, names of functions), name can be refered to from the scopes in other translation units. (variables/functions with external linkage have language-linkage (used for linking to translation units written in other languages)).
//		module linkage: (names declared at namespace scope have module linkage if their declarations are attached to a named module and are not exported, and don't have internal linkage) name can only be refered to from the scopes in the same module unit or in the other translation units of the same named module
//	}}}

//	anonymous function: a function definition not bound to an identifier (name)

//	functor: classes which implement 'operator()', creating objects which can be called like functions


//	Undefined behaviour: A nightmare, which may manifest as any of the following
//	{{{
//		program produces different results every time it is run.
//		program consistently produces the same incorrect result.
//		program behaves inconsistently (sometimes produces the correct result, sometimes not).
//		program seems like its working but produces incorrect results later in the program.
//		program crashes, either immediately or later.
//		program works on some compilers but not others.
//		program works until you change some other seemingly unrelated code.
//	}}}


//	Initialing a variable:
int value1 = 1; 		// copy initialization
double value2(2.2); 	// direct initialization
char value3 {'c'}; 		// uniform initialization


int main()
{
	return 0;
}

