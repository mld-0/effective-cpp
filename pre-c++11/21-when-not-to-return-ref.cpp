#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	TODO: 2022-01-30T02:40:13AEDT effective-c++, item 21, operators that return a reference (that isn't to '*this')?, (and how chained operators are (best) handled WRT to these returned objects/references?)

//	References are an alternative name for some other existing object
//	In the case of return by reference, consider what this object (actually) is (and does it already exist)
//	when chosing between return-by-value and return-by-reference, chose the option that gives the correct behaviour

class Rational {
public:
	Rational(int numerator=0, int denominator=1)		//	<see item 24 for why this isn't declared explicit>
		: n(numerator), d(denominator) 
	{}

private:
	int n, d;

	//	Ongoing: 2022-01-30T02:54:11AEDT the rule on the use of const (this being a return-by-value operator)?
	//	Ongoing: 2022-01-30T02:58:30AEDT use of 'friend', (when should) an operator be private?

	//	correct implementation (should it be private?) a <non-member> friend function? (this being better (support for order-of-arguemnts) than a member function (which would be correct to return a reference?)
	friend const Rational operator*(const Rational& rhs, const Rational& lhs) {
		return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
	}
};

//	bad attempts to implement 'operator*' for Rational

//const Rational& operator*(const Rational& rhs, const Rational& lhs) {
//	Rational result(lhs.n * rhs.n, lhs.d * rhs.d);		//	'result' is allocated on the stack
//	return result;										//	the returned reference is to a destroyed object
//}

//const Rational& operator*(const Rational& lhs, const Rational& rhs) {
//	Rational *result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);	//	'result' is a heap object
//	return *result;													//	client is responsible for calling delete
//}
//	This will cause memory leaks whenever temporary objects are in use, for example with the following expression:
//Rational w, x, y, z;
//w = x * y * z;

//	Nothing good is likely to come of trying to solve the problem by using a static variable (see book for example)

//	TODO: 2022-01-30T03:02:23AEDT effective-c++, item 21, <correct/best> form for declaring (each of the) operator functions, ((role of ) the non-member friend?), 
//	TODO: 2022-01-30T03:10:02AEDT effective-c++, item 21, example is when a function 'operator*' should return by value instead of by reference ((examples for) when should operators/functions return by reference?)

int main()
{
	Rational a(1,2);
	Rational b(3,5);
	//	Result is a rational object with values n=3, d=10. No such object exists before call, so it must be the job of the operator to create it
	Rational c = a * b;

	return 0;
}

//	Summary:
//		Never return a pointer or reference to a local stack object,
//		Never return a reference to a heap-allocated object, 
//		Never return a pointer or reference to a local static object if more than one such object might ever be needed

