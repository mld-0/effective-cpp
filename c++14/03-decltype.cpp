#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	decltype <C++11> Inspects the declared type of an entity, or the type and value category of an expression.
//	It is useful when declaring types that are difficult or impossible to declare using standard notation (lambda-related types, or types that depend on template parameters).
//	The primary use for decltype is declaring function templates whose return type depends on its parameter types

//	If the name of an object is parenthesized, it is treated as an ordinary lvalue expression, therefore:
//			decltype(x)
//			decltype((x))
//	may be different types.

//	decltype(auto) specifies that automatic type deduction should be performed using decltype rules.

//	For: 'decltype(x)'
//	If 'x' is the name of a variable (an id-expression), the resulting type is the type of that variable
//	Otherwise, 
//		if 'x' evalutes to an lvalue T, the resulting type is 'T&'
//		if 'x' evalutes to an rvalue T, the resulting type is 'T'
int foo() { return 0; }
int n = 10;
decltype(n) a = 20;					//	n is a id-expression, T = int
decltype((n)) b = a;				//	(n) is an lvalue, T = int&
decltype(foo()) c = foo();			//	foo() is an rvalue, T = int
decltype(foo()) && r1 = foo();		//	T = int&&
decltype((n)) && r2 = n;			//	T = int& (& && collapsed to &)

//	(Further) Examples:
class Widget {};
const int i = 0;
vector<int> v = {1,2,3};
bool f(const Widget& w);
Widget w;
decltype(i) var1 = 5;				//	T = const int
decltype(f) var2;					//	T = bool(const Widget&)
decltype(w) var3;					//	T = Widget
decltype(f(w)) var4;				//	T = bool
decltype(v) var5;					//	T = vector<int>
decltype(v[0]) var6 = v[0];			//	T = int&


//	Example: incorrect, auto return type deduction follows template type deduction rules, c[i] is a reference, and that reference-ness will be ignored. Passing by reference also prevents use of rvalue arguments.
template<typename Container, typename Index>
auto accessElement_i(Container& c, Index i) {
	return c[i];
}
//	Example: C++14 version of accessElement with decltype(auto) return type
template<typename Container, typename Index>
decltype(auto) accessElement_ii(Container&& c, Index i) {
	return std::forward<Container>(c)[i];
}
//	Example: C++11 version of accessElement with trailing decltype return type
template<typename Container, typename Index>
auto accessElement_iii(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i]) {
	return std::forward<Container>(c)[i];
}


//	Example: decltype(auto) return type and parenthesis
decltype(auto) f1() {
	int x = 0;
	return x;		//	return type = int
}
decltype(auto) f2() {
	int x = 0;
	return (x);		//	return type = int&
}



int main()
{
	vector<int> v1 = {1,23,4,7};
	//accessElement_i(v1, 1) = 1;			//	error, deduced return type is 'int'
	accessElement_ii(v1, 1) = 2;
	accessElement_iii(v1, 1) = 3;

	return 0;
}

//	Summary:
//		For a variable of type T, decltype return T given the variable name, T& given an lvalue, and T given an rvalue. (Note that 'x' is a name, whereas '(x)' is an lvalue).
//		decltype(auto) is like 'auto', but performs type deduction using decltype rules

