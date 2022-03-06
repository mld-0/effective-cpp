//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <initializer_list>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	For Vim with YCM, check deduced types with
//			:YcmCompleter GetType


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

//	Example: get_type_name<T>()
//	LINK: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#include <string_view>
template <typename T>
constexpr auto get_type_name() -> std::string_view {
//	{{{
#if defined(__clang__)
    constexpr auto prefix = std::string_view{"[T = "};
    constexpr auto suffix = "]";
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__GNUC__)
    constexpr auto prefix = std::string_view{"with T = "};
    constexpr auto suffix = "; ";
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__MSC_VER)
    constexpr auto prefix = std::string_view{"get_type_name<"};
    constexpr auto suffix = ">(void)";
    constexpr auto function = std::string_view{__FUNCSIG__};
#else
# error Unsupported compiler
#endif
    const auto start = function.find(prefix) + prefix.size();
    const auto end = function.find(suffix);
    const auto size = end - start;
    return function.substr(start, size);
}
//	}}}


int main()
{
	//	As for deducing template types, there are three cases:
	//	1) The type specifier is a pointer or a reference (but not a forwarding reference)
	//	Rule: If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T

	//	2) The type specifier is a forwarding reference
	//	Rule: If expr is an lvalue, both T and ParamType are deduced to be lvalue references (this is the only situtation where T is deduced to be a reference). If expr is an rvalue: If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T (that is, Case 1 rules apply)

	//	3) The type specifier is neither a pointer nor a reference (pass-by-value)
	//	Rule: If expr's type is a reference, ignore the reference part. If expr is const or volatile, ignore that too.

	auto x = 27;				//	T = int
	const auto cx = x;			//	T = int, ParamType = const int
	const auto& rx = x;			//	T = int, ParamType = const int&

	auto&& uref1 = x;			//	x is lvalue, T = int&
	auto&& uref2 = cx;			//	cx is lvalue, T = const int&

	auto&& uref3 = 27;			//	27 is rvalue, T = int&&		<(Being what the book says, but it isn't, YCM says T = int, and I can take its address? -> get_type_name() says 'int&&')>
	//	TODO: 2022-02-21T02:43:50AEDT effective-c++, item 02, auto-type-deduction, 'auto&& uref3 = 27' is T = int or T = int&& (and how to check?) -> (I can take its address (and I can change it) -> must be 'int'?) (where are the rules on this and what do they say?) (and if T = int, declare a 'uref4' for which T = int&&) (this is all to do with universal references and nothing to do with auto -> 'int&& uref3 = 27' <appears> to yield same result? meaning this is a topic for) (see item 24)

	//	Array and Function names decay into pointers for non-reference type specifiers
	const char name[] = "R. N. Briggs";
	auto arr1 = name;			//	T = const char*
	auto& arr2 = name;			//	T = const char (&)[13]
	//	void someFunc(int,double)
	auto func1 = someFunc;		//	T = void (*)(int,double)
	auto& func2 = someFunc;		//	T = void (&)(int,double)		<(YCM: void(int,double))>


	//	Example: when the initalizer for an auto variable is enclosed in braces {}, the deduced type is initalizer_list
	//	This is the <only?> difference between auto type deduction and template type deduction
	auto x1 = 27;				//	T = int
	auto x2(27);				//	T = int
	auto x3 = {27};				//	T = std::initalizer_list<int>
	auto x4{27};				//	T = std::initalizer_list<int>
	//	<(This is (one reason?) why there is no uniform new-C++ way to initalize variables)>


	//	Example: braces {} and template type deduction
	//		void f_i(T)
	//f_i( { 11,23,9 } );		//	error, cannot deduce T
	//		void f_ii(intializer_list<T>)
	f_ii( {11,23,9} );			//	T = int, ParamType = std::initalizer_list<int>


	//	Ongoing: 2022-02-20T18:24:32AEDT there is no non-lambda (function) with auto type arguments?
	//	Ongoing: 2022-02-20T18:22:59AEDT (presumedly) 'auto' for lambda return type follows same rules as auto for function return type?
	//	C++14 allows 'auto' as lambda parameter type (for which template type deduction rules are used).
	vector<int> v;
	auto resetV = [&v](const auto& newValue) { v = newValue; };
	//resetV( {1,2,3} );		//	error, cannot deduce type T

	(void) uref2; (void) uref3; (void) x1; (void) x3; (void) x2; (void) uref1; (void) rx; (void) arr2; (void) x4; (void) resetV; (void) arr1; (void) func1; (void) func2;
	return 0;
}

//	Summary:
//		auto type deduction follows the same rules as template type deduction, except for braces {}, which auto type deduction treats as an initalizer list (and template type deduction does not).
//		auto can be used for function return types and lambda 

