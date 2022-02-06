#include "personimpl.h"

PersonImpl::PersonImpl(const std::string& name, const Date& birthday, const Address& addr)
	: theName(name), theBirthDate(birthday), theAddress(addr)
{}

std::string PersonImpl::name() const { return theName; }
std::string PersonImpl::birthDate() const { return theBirthDate.to_string(); }
std::string PersonImpl::address() const { return theAddress.to_string(); }

int alksjdflksdjf;
