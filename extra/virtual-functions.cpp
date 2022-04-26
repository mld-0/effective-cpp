//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	TODO: 2022-04-25T01:35:32AEST effective-c++, extra/virtual-functions (what items cover them) (combining that material in one place?)
//	Ongoing: 2022-04-25T22:04:16AEST Base class pointers to Derived class objects (outside the context of virtual functions) (and in the context of the is-a relationship)
//	TODO: 2022-04-26T18:44:01AEST effective-c++, extra/virtual-functions, (the rule that says) only the return type of a pointer can be narrowed for a virtual function, (and such restrictions do not exist for overriding non-virtual functions (something we have an item saying not to do))

//	<about virtual functions>


//	<pure virtual functions>


//	<(A class with a virtual function should declare a virtual dtor. A class with a pure virtual function should declare a pure virtual dtor)>


//	<((on the question of) (needing to) providing a definition for pure virtual functions)> 
//	<(an elegent and *seemingly* correct(?) solution being to use '= default' as the definition of a pure virtual function (but why do we need to?))>
//	<(a dtor is the only pure virtual function for which a defintion is allowed?)>

//	Ongoing: 2022-04-25T21:50:13AEST example has virtual dtor to prevent warnings when used with smartpointer, please resolve (somewhere), (the question of) errors due to not providing a <definition/implementation> of a pure virtual dtor(?) [...] (providing a default virtual dtor is sufficent also) [...] '= default' is a better implementation for a pure virtual dtor than simply '{}'(?)
class Base {
public:    
	virtual ~Base() = 0;
    virtual void foo() = 0;
    virtual void bar() = 0;
};
Base::~Base() = default;

class Derived : public Base {
public:
    virtual void foo() override { printf("foo\n"); }
    virtual void bar() override { printf("bar\n"); }
    void foobar()  { printf("foobar\n"); }
};


int main()
{
	//	Example: Base class pointer to a Derived class
	Base* dv = new Derived();
	dv->foo();
	dv->bar();
	cout << "\n";

	//	Example: smartpointers also support virtual functions
	std::unique_ptr<Base> udv = std::make_unique<Derived>();
	udv->foo();
	udv->bar();
	cout << "\n";

	return 0;
}

