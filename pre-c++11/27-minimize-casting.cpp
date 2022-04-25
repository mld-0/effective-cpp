//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	Ongoing: 2022-04-26T00:14:10AEST (alternatives to casting away const?)

//	C++ attempts to eliminate type errors
//	In theory, a program that compiles cleanly is not trying to perform any unsafe or nonsensical operations on any objects

//	casts subvert the type system

//	Old-style casts (cast 'expression' to be of type T)
//		C style casts: 			(T) expression
//		Function style casts:	T(expression)
//	<(The C++ equivalent is the static_cast?)>

//	C++ has four new forms of cast:
//		static_cast<T>(expression)
//		dynamic_cast<T>(expression)
//		const_cast<T>(expression)
//		reinterpret_cast<T>(expression)
//	The new style casts should be prefered to the old style


//	static_cast: <The default new-style cast>. Force implicit conversions (eg: non-const T to T, int to double, void* pointer to typed pointer). Cannot cast away const.

//	dynamic_cast: cannot be performed with old-style cast. Used to perform 'safe-downcasting' (i.e: determine whether an object is of a particular type in an inheriteance hierachy). Can have a significant runtime cost

//	const_cast: cast away the constness of an object. (The official C++ guidelines state: do not cast away const)

//	reinterpret_cast: for low-level casts that yield implementation-dependent (unportable) results, eg: casting a pointer to an int. Should be rare outside low level code. 


//	An object may have a different address when pointed to be a different kind of pointer:
class Base {};
class Derived: public Base {};
Derived d;
Base *pb = &d;		//	implicitly convert Derived* to Base*
//	Sometimes, the value of the derived class pointer will be different to that of the base class pointer (by having an offset added to it at runtime) (most common with multiple inheritance (but can happen with single inheritence)). The behaviour of these offsets can vary from platform to platform. 

//	Casting an object to 'char*' and using pointer arithmetic on the result almost always yields undefined behaviour

//	Casting creates a copy of an object:
//	Don't cast 'this'. Casting an object (i.e: from derived class to base class) creates a temporary copy of the base class parts of that object. Calling functions on the result calls those functions for (and modifies) a copy of the object and not the object. 
//	The correct way to invoke a base class function from a derived class is:
class Window {
public:
	virtual void onResize() {}
};
class SpecialWindow: public Window {
public:
	virtual void onResize() {
		Window::onResize();
	}
};
//	If one is considering casting, it may be a sign that one is approaching the problem the wrong way, (especially for considering 'dynamic_cast')


//	Alternatives to dynamic_cast:
//	dynamic_cast is generally used to perform derived class operations on (what one believes to be) a derived class object which is only accessible through a base class pointer/reference
//	1) Use containers that store pointers (often smart pointers) to derived class objects directly (eliminating the need to manipulate the objects through base class interfaces). To work with multiple different derived classes, one may need multiple-type-safe containers for this approach. 
//	Ongoing: 2022-02-01T22:16:45AEDT multiple (pointer) type safe container example
//	2) Provide virtual function in the base class

//	Repeated use of dynamic_cast on the same object should be especially avoided (consider a virtual function solution instead)


int main()
{
	return 0;
}

//	Summary:
//		Avoid cast whenver practical (especially dynamic_casts in performance sensitive code)
//		Consider alternative approaches where casting is required
//		Where casting is necessary, hide it inside a function
//		Prefer new-style casts to old-style casts

