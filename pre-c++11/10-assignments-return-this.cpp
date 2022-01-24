#include <iostream>
#include <vector>
#include <string>
using namespace std;


int main()
{
	//	Assignments can be chained together, i.e:
	int x, y, z;
	x = y = z = 15;
	//	assignment like this is right-associative, i.e: equivalent to:
	x = (y = (z = 15));
	//	15 is assigned to z, the result of that assignment (updated value of z) is then assigned to y, and the updated value of y is then assigned to x
	//	for this to work, the assignment operator should return a reference to <its left hand argument (after updating it?)> (this is the '*this' for a member function operator)

	//	TODO: 2022-01-25T01:49:06AEDT effective-c++, applying the 'return *this' (i.e: reference to the LHS of the expresion(?)) rule for assignment operator to the two-argument form of the assignment operators

	//	(assignment operators returning a reference to (updated) (lhs <operand>) thrus) (is only a convention, that is a convention followed throughout the standard library) (following the standard library being the rule in C++ barring a good reason not to) ... (*assignment* (extended to-and-including <operator/(plus)> assignment) is only (and by definition) one argument)? ((what does the rule say about) addition/subtraction/multiplication/<a-class-of-operations-called-what?> returning <anything>?) (these operations (not assignments) being what may be either one (class method) or two (not-necesarily-friend) argument forms <> (making chained <such-operations> work (and returning something) work?)

	class Widget {
	public:
		Widget& operator=(const Widget& rhs) {
			//	...
			return *this;
		}
		Widget& operator+=(const Widget& rhs) {		//	<rule/convention> applies to +=, -=, *= (these being called what?)
			//	...
			return *this;
		}
		Widget& operator=(int rhs) {				//	rule applies even when using unconventionsal parameter type(s)
			return *this;
		}
	};

	return 0;
}

//	Summary:
//		assignment operators (being necessarily class member functions) should return a reference to '*this' (so they can be chained)

