#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Questions to consider when designing a class:
//		How should objects by created and destroyed?

//		How should initalization differ from assignment?

//		What does it mean to be passed by value? (behaviour of the copy constructor)

//		What are the restrictions on legal values? (restricted by constructors, assignment operator, and setter function, commonly involves being able to throw exceptions)

//		Does it fit into an inheritance graph? (inheritance design is restricted by what functions are declared virtual)

//		What type conversions are allowed? (implicit conversion by either implicit conversion operator (member of old type) or non-explicit constructor (member of new type)), (for explicit conversion, disable implicit conversion (declare constructors explicit) and provide custom conversion function)

//		What operators and functions make sense for type?

//		What standard member functions should be disallowed? <(declared private vs deleted?>)

//		Who should have access to class members? (what is public/protected/private, class friendship, nested classes)

//		What is the *undeclared interface*? (what guarantees does it offer WRT performance, exception safety, and resource usage)

//		How general is the type? (a family of types may be declared as a class template)

//		Is a new type really what is needed? (instead of deriving a new class to add functionality, consider instead a non-member function)

int main()
{
	return 0;
}

//	Summary:
//		Class design is type design. Consider the questions above.

