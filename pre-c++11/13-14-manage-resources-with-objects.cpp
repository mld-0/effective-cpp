#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Using objects to manage resources is best practice in C++, and is known as Resource Acquisition Is Initialization (RAII)
//	A single object should not be managing multiple resources
//	Destructors should release the resource, since dtors are called automatically whenever an object goes out of scope, the resource is released correctly regardless of how control leaves a block
//	Besides custom classes, smart pointers (auto_ptr (deprecated), shared_ptr, unique_ptr) can be used for this task
//	Using 'delete' outside of resource managing classes is usually a design mistake

//	Options for handling copy for resource managing classes:
//		1)	prohibit copying, (in many cases) it makes no sense to allow RAII objects to be copied
//		2)	reference counting, can be achieved by using a shared_ptr, it may be necessary to to supply a custom deleter function (an optional second parameter to shared_ptr constructor)
//		3)	(deep) copy the underlying resource, <>
//		4)	transfer ownership of the underlying resource, <this is different from a move how?> <how to nullify the source object during a copy?>

int main()
{
	return 0;
}

//	Summary:
//		Manage resources with objects that aquire the resource in their ctor and release it in their dtor
//		The properties of the underlying resource will determine how copying <can/should> be handled

