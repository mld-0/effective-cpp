#include <iostream>
#include <vector>
using namespace std;
//	TODO: 2022-04-26T00:07:09AEST effective-c++/07-polymorphic-base-class-virtual-destructors, (on the reasoning) it is necessary to provide a definition for a pure virtual dtor


//	Polymorphic base classes should declare virtual destructors. 
//	Any class that has virtual functions should have a virtual destructor. Conversely, a class with no virtual functions is probably not meant to be used as a base class
//	Deleting a derived class object through a base class pointer without a virtual destructor is undefined behaviour (in practice typically only the base class parts of the object are destroyed)

//	Virtual functions are associated with an increase in object size

//	<(If a function is virtual, all functions of the same name in derived classes are also virtual?)>

//	When calling a function of a derived class through a pointer to the base class, the function of that name corresponding to the correct type of the object will be called if the base function declares that function virtual
//	When overriding the virtual function of a base class, best practice is to declare the function of the derived class 'override
//	The STL container types do not have virtual destructors, (one) indication they are not meant to be used as base classes

//	A pure virtual function is a virtual function declared '= 0'. 
//	An abstract class is a function containing at least one pure virtual function. 
//	Instances of abstract classes cannot be initalized. A class is an abstract base class if it declares a virtual destructor 
//	<(Best practice is to give a class a pure virtual destructor if it has any pure virtual functions?)>
class Fruit {
public:
	virtual ~Fruit() = 0;
};
//	<required for earlier C++ versions / not just earlier versions?> 
//	It is <sometimes?> necessary to provide a definition for a pure virtual destructor:
//Fruit::~Fruit() {}
Fruit::~Fruit() = default;
//	Ongoing: 2022-04-25T22:30:05AEST implications of a non-empty definintion of a pure virtual dtor (presumedly Derived does call it?) 
//	Ongoing: 2022-04-25T22:32:46AEST (what are the rules vis-a-vis Derived calling Base dtors/ctors? [...] -> implicitly calls no-argument version, argument containing-version must be called <where?> in the <initalization list>) (see extra/inheritance-and-ctors-dtors)

//	When the time for destructor comes, the destructor of the most derived class is called first, then the destructor of each subiquent parent class in the hierachy

int main()
{
	return 0;
}

//	Summary:
//		If a class has any virtual functions, it should have a virtual destructor
//		<(If a class has any pure virtual functions, it should have a pure virtual destructor)>
//		If a class is not designed to be a base class (<or/that-is> be use polymorphically), it should not declare a virtual destructor

