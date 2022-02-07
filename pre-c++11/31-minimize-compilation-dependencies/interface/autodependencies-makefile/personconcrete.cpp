#include "personconcrete.h"

PersonConcrete::PersonConcrete(const std::string& name, const Date& birthday, const Address& addr) 
	: theName(name), theBirthDate(birthday), theAddress(addr)
{}

PersonConcrete::~PersonConcrete() {}

std::string PersonConcrete::name() const { return theName; }
std::string PersonConcrete::birthDate() const { return theBirthDate.to_string(); } 
std::string PersonConcrete::address() const { return theAddress.to_string(); }

