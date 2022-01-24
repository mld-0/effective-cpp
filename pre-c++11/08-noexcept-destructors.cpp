#include <iostream>
#include <vector>
using namespace std;

int main()
{
	//	Even though C++ permits destructors to throw exceptions, every effort should be made to prevent this, as a destructor throwing an exception can cause data corruption and crashes
	//	destructors that emmit excpetions do not play nicely with the standard library

	//	Destructors that cannot avoid exceptions have a few options (generally for each case it would be prudent to log the event)
	//		1)	Terminate the program, where possible, the safest option in forestalling undefined behaviour
	//		2)	Swallow the exception, problematic in that this suppresses information about the failure, information which is likely important. Should (only) be employed where the program is able to reliably continue from failures
	//		3)	Offer an interface function to perform the throwable action instead of the destructor, allows the client to handle the function, but relies on the client to remember to perform the action

	return 0;
}
