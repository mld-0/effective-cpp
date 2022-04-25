//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	Ongoing: 2022-04-25T22:35:44AEST (an implicit call to a Base dtor/ctor cannot take place where no no-argument version exists?)
//	Ongoing: 2022-04-25T22:36:27AEST calls to Base dtor/ctor in case of multiple inheritance?
//	Ongoing: 2022-04-25T22:46:21AEST is it possible to have a default Derived ctor/dtor where a non-default Base must be called?
//	Ongoing: 2022-04-25T22:48:23AEST (don't even try to) (but can one) (call multiple versions of a Base ctor/dtor?)
//	Ongoing: 2022-04-25T22:50:52AEST Base dtor/ctor must be called in the member initalization list (not in the body? can one even?) [...] (<significance-of/required> order of dtor/ctor call in member initalization list?)

//	LINKS:
//	{{{
//	LINK: https://stackoverflow.com/questions/10282787/calling-the-base-class-constructor-from-the-derived-class-constructor
//	LINK: https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-base-class-constructor
//	}}}

//	If a Derived dtor/ctor does not call a Base dtor/ctor explicitly, and a no-argument version is available, it call be called implicitly.
//	That is:
//			Derived() {}
//	and
//			Derived() : Base() {} 
//	are equivalent, provided 'Base()' is public.


class SuperClass {
public:
	SuperClass(int foo) {}
};
class SubClass : public SuperClass {
public:
	//	Where there is no no-argument Base dtor/ctor, the non-default version must be called explicitly
	//SubClass() {}

	SubClass(int foo)
		: SuperClass(foo), x(4) {}

	//	Produces warning (that x will be initalized after Base ctor is called, contrary to order of list)
	//SubClass(int foo, int bar)
	//	: x(5), SuperClass(foo) {}

	int x;
};


//	Catching exceptions in Base dtor/ctor:
class Base_i { 
public:
	Base_i(int x) {
		if (x == 0) { throw logic_error("x == 0"); }
	}
};
class Sub_i : public Base_i {
public:
	Sub_i(int x, int y)
	try : Base_i(x), member(y)
	{
		// function body goes here
	} catch(const exception& e) {
		//	<(unless another another exception is thrown before end of block, origional exception will be re-thrown - exceptions during <initalization/construction> cannot be ignored)>
		//	<((that is), if Base ctor/dtor throws an Exception (fails), Derived ctor/dtor must also)>
	}
	int member;
};


//	For multiple inheritance, the Base ctors/dtors are called in the order in which they are derived
class BaseA {
public:
	BaseA() { cout << "BaseA()\n"; }
};
class BaseB {
public:
	BaseB() { cout << "BaseB()\n"; }
};
class DerivedMulti_i: public BaseA, public BaseB {
public:
	DerivedMulti_i(): BaseA(), BaseB() {}

	//	Produces warning, that 'BaseB()' is actually called before 'BaseA()', contrary to order of list
	//DerivedMulti_i(): BaseB(), BaseA() {}
};


int main()
{
	Sub_i s1(2,3);
	try {
		Sub_i s2(0, 0);
	} catch (const exception& e) {
		cout << "e.what()=(" << e.what() << ")\n";
	}
	cout << "\n";

	DerivedMulti_i d1;
	cout << "\n";

	return 0;
}

