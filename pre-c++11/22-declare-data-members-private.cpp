#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Declare (all) class member variables private. Advantages include:

//	Consistency, if there are no public variables, we can presume any public member is a function

//	Get and set functions allow greater control over public variables

//	Encapsulation, (which is) is inversly proportional to the amount of code that might be broken if something changes.  If member variables are hidden, they can later be replaced when making later changes to the class
//	(alternatively, anything that is public cannot be changed)
//	hiding object variables from clients prevents clients from invalidating the internal state of objects.
//	Public means unencapsulated (and unencapsulated means unchangable)

//	The same arguments against public variables apply to protected variables: protected variables are just as unencapsulated as public variables, and it is just as important to preserve encapsulation within class hierachies

int main()
{
	return 0;
}

//	Summary:
//		Declare class data members private. It provides encapsulation, consistency (all public members are functions), fine-control over object state, the enforcement of invariants, and allows for later changes to the implementation of the class
//		Protected variables are no more encapsulated than private variables

