#include "personinterface.h"
#include "personconcrete.h"

//	empty definition must be provided for pure virtual destructor to use unique_ptr as class member
PersonInterface::~PersonInterface() {}

//	factory function: implemented in terms of 'PersonConcrete' implementation class
std::unique_ptr<PersonInterface> PersonInterface::create(const std::string& name, const Date& birthday, const Address& addr) 
{
	//	using this technique, one may return a different type of derived implementation class depending on argument values
	return std::unique_ptr<PersonInterface>(new PersonConcrete(name, birthday, addr));
}

