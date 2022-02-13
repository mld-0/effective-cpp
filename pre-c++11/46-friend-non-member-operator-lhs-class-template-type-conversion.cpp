#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	(item 24) explains that only two-argument non-member forms of operator functions are eligible for implicit type conversion of both lhs and rhs arguments. 

template<typename T>
class Rational {
public:
	Rational(const T& numerator = 0, const T& denominator = 0)
		: n(numerator), d(denominator)
	{}

	const T numerator() const { return n; }
	const T denominator() const { return d; }

	//	Note that the purpose of friendship here is not to access non-public parts of the class: the only way to declare a non-member function inside the body of a class is to make it a friend. (If having the operator function inline is a problem, declare the implementation outside the class and call that). This declaration inside the class is necessary to support mixed-type calls to the function.
	friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
		return doMultiply(lhs, rhs);
	}

	//	Inside a class template, the class name can be used as shorthand for the template and its parameters: inside the class, 'Rational' can be used in place of 'Rational<T>'
	//friend const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs);

private:
	T n;
	T d;
};

//	When trying to resolve the template type parameter T, implicit conversion is not taken into account, and T cannot be resolved for multiplication calls of mixed type. The solution is to declare the operator function a friend of the class (and provide the definition inside the class, so the linker can find it). 
//	Class templates don't depend on template argument deduction, T is always known at the time Rational<T> is instantiated. If the template operator function is declared a friend, it is automatically instantiated along with the class. Since it is already declared, implicit conversion can be used to call it, and mixed type uses of the operator can compile.
//template<typename T>
//const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
//	return Rational<T>(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
//}
//	Unless we place the body of our operator* function inside the class body <(the linker cannot find it)>


//	Optionally we place operator implementation outside the body of the class (so that it is not inline). This function template doesn't support mixed-type multiplication (but doesn't need to, since it is being called by operator*() which does support mixed type multiplication via implicit conversion - because we made it a friend of the class)
template<typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs) {
	return Rational<T>(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

int main()
{
	Rational<int> oneHalf(1,2);

	//	Implicit type conversion is never considered during template argument deduction. 
	//	If the operator function is not declared a friend of the class, there is no way to resolve T given the calls:
	Rational<int> r1 = oneHalf * 2;
	Rational<int> r2 = 2 * oneHalf;

	return 0;
}

//	Summary:
//		When writing a class template that offers operator functions supporting implicit type conversion on all parameters, these must be non-member functions (see item 24). The template type parameter cannot be determined for these functions unless they are declared a friend of the relevant class (hence instantiating the function along with the class template, at which point implicit type conversion can take over), and the compiler cannot link the implementation unless it is also provided inside the class body.

