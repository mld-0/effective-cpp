#include "date.h"			
#include "address.h"
#include "personinterface.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	unique_ptr<PersonInterface> y = PersonInterface::create("name value", Date("date value"), Address("address value"));
	cout << "y->name()=(" << y->name() << "), y->birthDate()=(" << y->birthDate() << "), y->address()=(" << y->address() << ")\n";

	return 0;
}

