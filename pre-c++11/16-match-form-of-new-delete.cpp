#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	In C++, 'new' / 'new[]' and 'delete' / 'delete[]' are different functions. 
//	For an object created with one form, the corresponding form must be used in its deletion
//	If there are multiple functions that might be used to initalize a particular piece of memory with 'new', they should all use the same form of new (avoiding ambiguity on which form of delete must be used)

int main()
{
	//	It is best to avoid typedefs of array types (since doing otherwise requires documenting which form of delete must be used) (which may not match that used during creation), eg:
	typedef string AddressLines[4];
	//string* pal = new AddressLines[5];	//	invalid
	string* pal = new AddressLines;
	//delete pal		//	wrong (and undefined)
	delete [] pal;

	//	(In any case) the std containers (mostly) eliminate the need for dynamically allocated arrays

	return 0;
}

//	Summary:
//		If '[]' is used with 'new', it must also be used in the corresponding delete, and vice-versa

