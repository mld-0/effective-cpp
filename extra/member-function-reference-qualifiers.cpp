#include <iostream>
#include <vector>
#include <string>
using namespace std;


//	Example: Member function reference qualifiers (from item 12)
class ExampleReferenceQualifiers {
public:
	vector<int> data() & {					//	invoked when '*this' is an lvalue
		return this->values;
	}
	vector<int>&& data() && {				//	invoked when '*this' is an rvalue
		return std::move(this->values);
	}
	//	Ongoing: 2022-04-25T23:44:39AEST <(providing neither lvalue/rvalue reference qualifiers defines the function for instances of both, (or conversely, defining with lvalue/rvalue reference qualifier restricts function to one-or-the-other) [...] (presumedly where with/without are provided, a matching reference qualified version is prefered?))>
private:
	vector<int> values;
};


int main()
{
	return 0;
}

