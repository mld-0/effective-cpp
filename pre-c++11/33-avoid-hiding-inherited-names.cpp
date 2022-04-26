//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	TODO: 2022-04-26T18:40:13AEST effective-c++, 33-avoid-hiding-inherited-names, (clarify *rules* on <hiding/overriding> non-virtual functions) (refer to extra/ownership-with-smart-pointers, and the 'workaround' since one cannot narrow the return type of a unique_ptr like one can narrow the return type of a pointer for an overridden virtual function (the workaround being to use a non-virtual function and override that, achieving polymorphic behaviour by calling a private virtual function which returns the raw pointer for the creation of the smartpointer))

//	Scope:
//	LINK: https://en.cppreference.com/w/cpp/language/scope
//	Each name that appears in C++ is only visible in some (possibly discontiguous) porition of the source code, that being it's scope. (within a scope, unqualified name lookup <can be used> to associate a name with its declaration) <Scopes are generally given by braces {}>.
//	There are six kinds of scope: Global, Namespace, Local, Class, Statement, Function
//	Names in inner scopes hide (shadow) names in outer scopes. A function from an inner scope will be chosen over an outer scope function of the same name even if the outer scope function better matches the type(s) in question.

//	Derived classes inherit the things declared in base classes. How this works: the scope of a derived class is nested inside the scope of the base class.


class Base {
private:
	int x;
public:
	virtual void mf1() = 0;
	virtual void mf1(int);
	virtual void mf2();
	void mf3();
	void mf3(double);
};
class Derived : public Base {
public:
	virtual void mf1();
	void mf3();
	void mf4();
};
void Derived::mf4() {
	mf2();		// calls 'Base::mf2()'
}
//	Scope of 'Base' (shared with 'Derived')
//		int x
//		void mf1()	2 functions
//		void mf2()	1 function
//		void mf3()	2 functions
//	Scope of 'Derived'
//		void mf1()	1 function
//		void mf3()	1 function
//		void mf4()	1 function
//	In the Derived class, all 'mf1()' functions declared in Base are being hidden by the fact there is at least one function of the same name. That is, from the perspective of name lookup, Base::mf1() and Base::mf3() are no longer inherited by Derived. This hiding behaviour happens regardless of parameter type and virtual-ness.

//	Under public inheritance, not inheriting all functions (and all their overloads) from a base class violates the 'is a' rule. This hiding of inherited names can be overridden by using declarations, that is adding to the Derived class: (for each each name that would otherwise be hidden)
//	'using Base::mf1;'

//	For older compilers (which do not support 'using' declarations), 'forwarding' functions is an alternative way of making a Base class function available:

int main()
{
	return 0;
}

//	Summary:
//		Names in derived classes hide names in base classes. Under public inheritance this is never desirable.
//		To make such hidden names visible, use 'using' declarations, or forwarding functions

