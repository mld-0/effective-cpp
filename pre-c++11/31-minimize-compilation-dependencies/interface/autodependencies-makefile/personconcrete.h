#include <string>
#include "date.h"
#include "address.h"
#include "personinterface.h"	//	required(?)

#ifndef __PERSONCONCRETE_H
#define __PERSONCONCRETE_H

class PersonConcrete: public PersonInterface {
public:
	PersonConcrete(const std::string& name, const Date& birthday, const Address& addr);
	~PersonConcrete();
	std::string name() const override;
	std::string birthDate() const override;
	std::string address() const override;
private:
	std::string theName;
	Date theBirthDate;
	Address theAddress;
};

#endif

