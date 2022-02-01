#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	References, pointers, and iterators are all handels, and returning a handle to an objects internals always runs the risk of compromising an objects encapsulation.
//	This also applies to protected/private member functions as well as variables

//	Whenever a handle is being returned, one is running the risk that the handle will outlive the object it referes to

//	Ongoing: 2022-02-01T22:42:58AEDT <A bad example, reference to Point (const but able to modify object)>

//	<Implications:>
//	A data member is only as encapsulated as the most accessible function returning a reference to it
//	If a const member function returns a reference to data associated with the object that is stored outside the object itself, the caller can modify that data (limitation of bitwise const-ness, see item 03) (to prevent this return a const ref)

//	A member function should never return a pointer to a less-accessible member function

//	Ongoing: 2022-02-01T22:42:45AEDT <another example, taking the pointer of a handle returned by a temporary object?>

int main()
{
	return 0;
}

//	Summary:
//		Avoid returning handles (references, pointers, or iterators) to object internals.
//		Not returning handles helps increases encapsulation, helps const member functions act const, and minimizes the chances of dangling handles

