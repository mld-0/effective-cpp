//#include "personimpl.h"
#include <string>

//	<(why aren't we importing our declaration? is it really <worth-it/necessary> to declare 'personimplfwd'?)>
class PersonImpl;						//	forward declaration
class Date;
class Address;

class Person {
public:
	Person(const std::string& name, const Date& birthday, const Address& addr);
	~Person();
	std::string name() const;
	std::string birthDate() const;
	std::string address() const;
private:
	std::unique_ptr<PersonImpl> pImpl;		//	smartpointer to implementation
};

