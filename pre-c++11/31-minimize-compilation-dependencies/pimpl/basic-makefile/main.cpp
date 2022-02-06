#include "date.h"			//	Ongoing: 2022-02-05T00:33:31AEDT importing 'date.h' / 'address.h' undermines example?
#include "address.h"
#include "person.h"
#include <iostream>
#include <string>
using namespace std;

//	TODO: 2022-02-05T00:02:06AEDT (who gets the *fwd) header?

int main()
{
	Person x("name-value", Date("date-value"), Address("address-value"));
	cout << "x.name()=(" << x.name() << "), x.birthDate()=(" << x.birthDate() << "), x.address()=(" << x.address() << ")\n";

	return 0;
}
