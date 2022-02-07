#include "date.h"
#include "address.h"
#include <string>

class PersonImpl {
public:
	PersonImpl(const std::string& name, const Date& birthday, const Address& addr);
	std::string name() const;
	std::string birthDate() const;
	std::string address() const;
private:
	std::string theName;
	Date theBirthDate;
	Address theAddress;
};

