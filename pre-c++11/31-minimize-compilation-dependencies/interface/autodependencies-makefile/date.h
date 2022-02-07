#include <string>

class Date {
public:
	Date(std::string v);
	std::string to_string() const;
private:
	std::string val;
};

