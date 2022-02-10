#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Never redefine an inherited non-virtual function

//	Non-virtual functions are statically bound. (If a non-virtual function of Derived object is invoked through a Base class pointer, and the Derived class overrides the Base class function, the Base class function is invoked).
//	(Invoking a non-virtual function on a Base pointer to a Derived object will call the Base version of that function, even if Derived re-defines it)
//	(Redefining an inherited non-virtual function is likely to be a source of inconsistent behaviour)
//	(A non-virtual function defines an invariant over specialization for all Derived classes)
//	(so (such a redefinition is) a contradiction in the 'is a' relationship) (what about the case of private inheritance?)

class A {
public:
	void stuff() { cout << "A::stuff\n"; return; }
};
class B: public A {
public:
	double stuff() { cout << "B::stuff\n"; return 7; }
};

int main()
{
	B item1;
	A* pitem1 = &item1;

	item1.stuff();		//	invokes B::stuff()
	pitem1->stuff();	//	invokes A::stuff() 
	//	Ongoing: 2022-02-10T15:20:55AEDT function invoked has return type 'double', but compiler thinks it has type 'void' (this no doubt being bad) (but what are the details vis-a-vis how bad?)

	return 0;
}

//	Summary:
//		Never redefine an inherited non-virtual function

