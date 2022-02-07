#include <string>

class Address {
public:
	Address(std::string v);
	std::string to_string() const;
private:
	std::string val;

};

