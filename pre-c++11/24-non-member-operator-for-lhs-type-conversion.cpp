#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Rational {
public:
	//	Constructor defines an implicit conversion from int->Rational
	Rational(int numerator=0, int denominator=1) : n(numerator), d(denominator) {}
	int numerator() const { return n; }
	int denominator() const { return d; } 

	//	If 'operator*' is implemented as a class method (where lhs is 'this')
	//const Rational operator*(const Rational& rhs) const;
	//Rational oneEight(1,8);
	//Rational oneHalf(1,2);
	//Rational result = oneHalf * oneEight;		//	valid
	//result = oneHalf * 2;						//	valid
	//result = 2 * oneHalf;						//	invalid

	//	By declaring the 'operator*' function a class member, type converison (even implicit type conversion) can only take place for the RHS variable. Consider the alternative forms of the examples above:
	//result = oneHalf.operator*(2);
	//result = 2.operator*(oneHalf);

private:
	int n, d;
};

//	This issue is solved by declaring the 'operator*' function as a non member function (avoid making it a friend unless actually necessary). This allows implicit type conversion for both lhs/rhs
const Rational operator*(const Rational& lhs, const Rational& rhs) {
	return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

int main()
{
	//	All valid with non-member operator function
	Rational oneEight(1,8);
	Rational oneHalf(1,2);
	Rational result = oneHalf * oneEight;
	result = oneHalf * 2;					
	result = 2 * oneHalf;						
	return 0;
}

//	Summary:
//		If the operator function for a class needs to support type conversion on either the LHS or RHS, it must be implemented a non-member function (such an operator implemented as a class member-function does not allow implicit type conversion for the LHS (the 'this' pointer for such a function). 
//		Functions should only be class friends if they actually need to be 

