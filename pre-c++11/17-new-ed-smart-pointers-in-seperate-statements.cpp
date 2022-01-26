#include <iostream>
#include <vector>
#include <iostream>
using namespace std;

class Widget {};
void processWidget(shared_ptr<Widget> w, int priority) { }
int getPriority() { return 0; }

int main()
{
	//	bad and invalid:
	//processWidget(new Widget, 0);		// 	no implicit conversion from raw pointer to shared_ptr

	//	bad:
	processWidget(shared_ptr<Widget>(new Widget), getPriority());	
	//	before 'processWidget()' can be called, three statements must be executed: 1) 'new Widget', 2) shared_ptr constructor, and 3) 'getPriority()'
	//	1 must come before 2, but it is up to the compiler whether 3 is executed before, after, or between these operations. If 'getPriority()' throws an exception, it is possible that a Widget will be created but never assigned to the shared_ptr meant to delete it, causing a leak

	//	For this reason, whenever a smartpointer is being created from a dynamically allocated object, it should be done in a single statement with no other operations

	//	good:
	shared_ptr<Widget> pw(new Widget);
	processWidget(pw, getPriority());

	return 0;
}

//	Summary:
//		When storing 'new'-ed objects in smart pointers, do so in a standalone statement

