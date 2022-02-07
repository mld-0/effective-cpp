#include "person.h"					//	implementing person class, must include its definition
#include "personimpl.h"				//	can't call PersonImpl member functions without definitions

Person::Person(const std::string& name, const Date& birthday, const Address& addr)
	: pImpl(new PersonImpl(name, birthday, addr))		//	note the use of 'new'
{}

//	if the default deleter for unique_ptr is used, the type used in the unique_ptr must be complete where this deleter is invoked: for this reason we define the default destructor in the cpp file
Person::~Person() = default;

std::string Person::name() const { return pImpl->name(); }
std::string Person::birthDate() const { return pImpl->birthDate(); }
std::string Person::address() const { return pImpl->address(); }
