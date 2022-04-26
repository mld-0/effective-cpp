#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	For a function to be exception safe, when an exception is thrown, it should:
//		1)	leak no resources
//		2)	not allow data structures to becomes corrupted

//	Three guarantees of function exception safety:
//		The basic guarantee: if an exception is thrown, the program remains in a valid state
//		The strong guarantee: if an exception is thrown, the state of the program is unchanged (function is atomic)
//		The nothrow guarantee: a promise to never throw exceptions for an operation that cannot fail

//	One should offer the strongest guarantee that is practical

//	Examples of code that should be no-throw: (must ignore or handle errors through other means)
//		destructors and memory deallocation/cleanup functions
//		functions that higher-level no-throw functions need to call

//	Examples of code that should be no-fail: (must always succeed)
//		move constructors and move assignment (move semantics, covered in chapter M)
//		swap functions
//		clear/erase/reset functions on containers
//		operations on std::unique_ptr 
//		functions that higher-level no-fail functions need to call


//	Ongoing: 2022-04-01T19:23:33AEDT is this 'set_unexpected' specific to <C++98> exception specifications?
int doSomething() throw(); //	If 'doSomething()' does throw an exception, call the 'unexpected' function (see 'set_unexpected')

//	avoid changing the status of an object to indicate that something has happened until it actually has

//	The strong exception guarantee is commonly implemented with the copy-and-swap idiom (see extra/copy-and-swap)

//	A function can (usually) offer a exception guarantee no stronger than the weakest guarantee of the functions it calls

//	Legacy code commonly lacks exception safety. Consider exception safety when creating or modifying code. Use objects to manage resources, consider which guarantee is the strongest that can be practically offered, settle for no guarantee only if calls to legacy code leave no choice.

//	Example: 'set_unexpected()'

int main()
{
	return 0;
}

//	Summary:
//		Exception safe functions leak no resources and allow no data structures to becomes corrupt when exceptions are thrown
//		The strong guarantee is commonly implemented with copy-and-swap
//		A function can usually offer a guarantee no stronger than the weakest guarantee of the function it calls

