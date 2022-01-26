#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Custom copy-constructors and copy-assignments are used where the default versions are unsuitable
//	The compiler does not provide warnings where a custom copy ctor/assignment function does not copy all data members
//	This lack of warning makes it easy to forget to add subsiquently created data members to the copy functions
//	It is also easy to forget to copy data members of the base class when writing the copy functions of a derived class

//	derived class copy functions should invoke their corresponding base class functions
class Date {};
class Customer {
public:
	//	...
private:
	string name;
	Date lastTransaction;
};
class PriorityCustomer : public Customer {
public:
	PriorityCustomer(const PriorityCustomer& rhs) 
		: Customer(rhs), priority(rhs.priority)		//	invoke base class copy ctor
	{
	}
	PriorityCustomer& operator=(const PriorityCustomer& rhs) 
	{
		Customer::operator=(rhs);	//	assign base class data members
		priority = rhs.priority;
		return *this;
	}
private:
	int priority;
};

//	neither the copy-constructor or copy-assignment functions should be declared in terms of the other, instead move any common code to a private function 'init()'

int main()
{
	return 0;
}

//	Summary:
//		Be sure that copy functions copy all of an objects data members, and all of its base class members
//		Do not implement either the copy-ctor or copy-assignment in terms of the other

