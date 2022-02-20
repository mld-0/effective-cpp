#include <initializer_list>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Given function template:
//			template<typename T>
//			void f(ParamType param);
//	Called:
//			f(expr);
//	'T' and 'ParamType' are deduced from 'expr'

//	(With the exception of values in braces), type deduction for auto variables follows the same rules as for templates.
//			auto x = 27;
//			const auto cx = x;
//			const auto& rx = x;
//	Here 'auto' plays the role of 'T', and 'ParamType' is 'auto', 'const auto', and 'const auto&' respectively


void someFunc(int a,double b) { cout << "someFunc(" << a << "," << b << ")\n"; }

template<typename T> 
void f_i(T param) {}

template<typename T>
void f_ii(std::initializer_list<T> param) {} 


//	C++14 allows 'auto' as function return type (for which template type deduction rules are used).
//auto createInitList() { return {1,2,3}; }			//	error, can't deduce return type


int main()
{
	//	As for deducing template types, there are three cases:
	//	1) The type specifier is a pointer or a reference (but not a forwarding reference)
	//	If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T

	//	2) The type specifier is a forwarding reference
	//	If expr is an lvalue, both T and ParamType are deduced to be lvalue references (this is the only situtation where T is deduced to be a reference).
	//	If expr is an rvalue: If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T (that is, Case 1 rules apply)

	//	3) The type specifier is neither a pointer nor a reference (pass-by-value)
	//	If expr's type is a reference, ignore the reference part. If expr is const or volatile, ignore that too.

	auto x = 27;				//	T = int
	const auto cx = x;			//	T = int, ParamType = const int
	const auto& rx = x;			//	T = int, ParamType = const int&

	auto&& uref1 = x;			//	x is lvalue, T = int&
	auto&& uref2 = cx;			//	cx is lvalue, T = const int&
	auto&& uref3 = 27;			//	27 is rvalue, T = int&&	
	//	Ongoing: 2022-02-20T17:52:26AEDT non-const-ness of a 'constant' value? (uref3 can be changed (I checked))

	//	Array and Function names decay into pointers for non-reference type specifiers
	const char name[] = "R. N. Briggs";
	auto arr1 = name;			//	T = const char*
	auto& arr2 = name;			//	T = const char (&)[13]
	//	void someFunc(int,double)
	auto func1 = someFunc;		//	T = void (*)(int,double)
	auto func2 = someFunc;		//	T = void (&)(int,double)


	//	Example: when the initalizer for an auto variable is enclosed in braces {}, the deduced type is initalizer_list
	//	This is the <only?> difference between auto type deduction and template type deduction
	auto x1 = 27;				//	T = int
	auto x2(27);				//	T = int
	auto x3 = {27};				//	T = std::initalizer_list<int>
	auto x4{27};				//	T = std::initalizer_list<int>
	//	<(This is (one reason?) why there is no uniform new-C++ way to initalize variables)>


	//	Example: braces {} and template type deduction
	//	void f_i(T)
	//f_i( { 11,23,9 } );		//	error, cannot deduce T
	//	void f_ii(intializer_list<T>)
	f_ii( {11,23,9} );			//	T = int, ParamType = std::initalizer_list<int>


	//	Ongoing: 2022-02-20T18:24:32AEDT there is no non-lambda (function) with auto type arguments?
	//	Ongoing: 2022-02-20T18:22:59AEDT (presumedly) 'auto' for lambda return type follows same rules as auto for function return type?
	//	C++14 allows 'auto' as lambda parameter type (for which template type deduction rules are used).
	vector<int> v;
	auto resetV = [&v](const auto& newValue) { v = newValue; };
	//resetV( {1,2,3} );		//	error, cannot deduce type T

	return 0;
}

//	Summary:
//		auto type deduction follows the same rules as template type deduction, except for braces {}, which auto type deduction treats as an initalizer list (and template type deduction does not).
//		auto can be used for function return types and lambda 

