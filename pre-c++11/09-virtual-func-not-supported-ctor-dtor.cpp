#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	virtual functions aren't virtual when called from a constructor/destructor. Constructors and destructors should not call virtual functions (the base class is initalized before the derived class during construction and therefore it is meaningless to call a derived function from a base class) (the reverse applies to destructors)
//	this behaviour can be difficult to detect when ctors/dtors call class functions which in turn call virtual functions

//	alt solutions for polymorphic behaviour from ctor/dtors:
//		~~1) generate class specific data in derived class constructor and pass to base class destructor~~
//		1)	generate class specific data in derived class <(static)?> function and pass to base class constructor, <(passing information up to base class instead of down from derived class)>
class Transaction {
public:
	explicit Transaction(const string& logInfo) {
		cout << "Transaction constructor, logInfo=(" << logInfo << ")\n";
		logTransaction(logInfo);
	}
	void logTransaction(const string& logInfo) const {
		cout << "Transaction, log logInfo=(" << logInfo << ")\n";
	}
};
class BuyTransaction: public Transaction {
public:
	BuyTransaction(int x)
		: Transaction(createLogString(x)) {
		cout << "BuyTransaction constructor, x=(" << x << ")\n";
	}
private:
	static string createLogString(int x) {
		return "BuyTransaction, createLogString x=(" + to_string(x) + ")";
	}
};

//		2)	<?> Ongoing: 2022-01-25T01:05:20AEDT another option (does the book (not) state there are 'several' options yet (only presents one)?)


int main()
{
	BuyTransaction a(3);

	return 0;
}
