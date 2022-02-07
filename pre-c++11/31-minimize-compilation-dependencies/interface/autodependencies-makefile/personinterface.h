#include <string>

//	Only a forward-declaration is needed, avoid unnecessary imports
class Date;
class Address;

#ifndef __PERSONINTERFACE_H
#define __PERSONINTERFACE_H

class PersonInterface {
public:
	virtual ~PersonInterface() = 0;		//	must have a pure virtual destructor with empty definition to use unique_ptr as class member

	//	Virtual Functions define interface
	virtual std::string name() const = 0;
	virtual std::string birthDate() const = 0;
	virtual std::string address() const = 0;

	//	factory function (returns derived implementation class as unique_ptr to interface class)
	static std::unique_ptr<PersonInterface> create(const std::string& name, const Date& birthday, const Address& addr);

private:
};

#endif

