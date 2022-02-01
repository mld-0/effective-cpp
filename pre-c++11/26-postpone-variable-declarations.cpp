#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	A variable should not be declared until right before it is about to be used
//	Ideally not until there is a value with which to initalize it (skipping the cost of a default construction)

class Widget {};

int main()
{
	//	considering the case of loops (is this not a question best left to the compiler) <(by which I mean variables should be declared in the loop and not before the loop (all other things being equal, to limit their scope to only where they are needed))>

	//	Define variable before loop: 1 ctor + 1 dtor + n assignments
	Widget w;
	for (int i = 0; i < 10; ++i) {
		//	w = ...
	}

	//	Define variable inside loop: n ctors + n dtors
	for (int i = 0; i < 10; ++i) {
		Widget w; // = ...
	}

	return 0;
}

//	Summary:
//		Postpone variable declarations as long as possible (to increase clarity and efficency)

