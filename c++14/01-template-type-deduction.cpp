#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	For Vim with YCM, check deduced types with
//			:YcmCompleter GetType

//	C++98 has a single set of rules for type deduction: those for function templates. C++11 adds rules for auto, and rules for decltype. C++14 extends the contexts in which these later two may be employed.

//	Type deduction can make the types of variables in codes not as apparent as one might like.

//	For a function template has the form:
//			template<typename T>
//			void f(ParamType x);
//	That is called with:
//			f(expr);
//	During compilation, 'expr' is used to deduce two types: one for 'T', and one for 'ParamType'

//	For example: 
//		template<typename T>
//		void f(const T& param) { cout << "param=(" << param << ")\n"; }
//	When 'f(x)' is called for 'int x = 0', ParamType is deduced to be 'const T&' and T is deduced to be 'int'.


//	TODO: 2022-02-20T02:36:19AEDT effective-c++, item 01, template-type-deduction, descriptive names for f_.* functions

template<typename T> void f_i(T& param) { cout << "f_i(" << param << ")\n"; }
template<typename T> void f_ii(const T& param) { cout << "f_ii(" << param << ")\n"; }
template<typename T> void f_iii(T* param) { cout << "f_iii(" << *param << ")\n"; }
//	Ongoing: 2022-02-20T01:53:30AEDT <the-case-of> const-pointer and pointer-to-const
template<typename T> void f_iv(T&& param) { cout << "f_iv(" << param << ")\n"; }
//	Ongoing: 2022-02-20T01:29:32AEDT (about <the-case-of>) const universal reference?
template<typename T> void f_v(const T&& param) { cout << "f_v(" << param << ")\n"; }
template<typename T> void f_vi(T param) { cout << "f_vi(" << param << ")\n"; }
template<typename T> void f_vii(const T param) { cout << "f_vii(" << param << ")\n"; }

//	Example: using (the ability to declare references to arrays) to create template function return size of an array as a compile-type constant. 'constexpr' makes results available during compilation and allows additional optimizations (see item 15). 
template<typename T, std::size_t N> 
constexpr std::size_t compileTimeArraySize(T (&)[N]) noexcept { return N; }


//	Function types can (also) decay into function pointers.
void someFunc(int a, double b) { cout << "someFunc(" << a << "," << b << ")\n"; }
template<typename T>
void f1(T param) { cout << "f1(" << param << ")\n"; }
template<typename T>
void f2(T& param) { cout << "f2(" << param << ")\n"; }

int main()
{
	//	There are three cases for 'ParamType': 1) a pointer or reference but not a universal (forwarding) reference, 2) a (forwarding) universal reference, 3) neither a pointer nor reference

	//	Case 1) ParamType is a pointer, or a non-universal (non-forwarding) reference
	//	Rule: If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T
	int x = 27;					//	int
	const int cx = x;			//	const int
	const int& rx = x;			//	reference to const int
	const int* px = &x;			//	pointer to const int
	const int* const cpx = &x;	//	const pointer to const int
	//	void f_i(T&)
	//	Passing a const object to a template taking a 'T&' parameter is safe, because the const-ness of the object is deduced as part of type 'T'
	//	Ongoing: 2022-02-21T03:31:52AEDT does 'template<>' not come into deduction/T/ParamType?
	f_i(x);						//	T = int, ParamType = int&
	f_i(cx);					//	T = const int, ParamType = const int&
	f_i(rx);					//	T = const int, ParamType = const int&
	f_i(*px);					//	T = const int, ParamType = const int&
	f_i(*cpx);					//	T = const int, ParamType = const int&
	//f_i(27);					//	invalid


	//	void f_ii(const T&)
	f_ii(x);					//	T = int, ParamType = const int&
	f_ii(cx);					//	T = int, ParamType = const int&
	f_ii(rx);					//	T = int, ParamType = const int&
	f_ii(*px);					//	T = int, ParamType = const int&
	f_ii(27);					//	T = int, ParamType = const int&
	//	void f_iii(T*)
	f_iii(&x);					//	T = int, ParamType = int*
	f_iii(px);					//	T = const int, ParamType = const int*
	f_iii(cpx);					//	T = const int, ParamType = const int*	<(does 'const int* const' not come into it -> (this is a copy of the unchangeable pointer that can be changed (in the function)) (YCM says 'T = const int, ParamType = const int*' (with no second const) (which we do get for type of 'cpx')))>
	//f_iii(27);				//	invalid
	cout << "\n";

//	TODO: 2022-02-21T03:37:50AEDT effective-c++, item 01, template-type-deduction, (what is the meaning of) '&*px' (address of the value of a pointer) for 'int x = 27; const int* px = &x;'?


	//	Case 2) ParamType is a universal (forwarding) reference, preserves the lvalue/rvalue-ness of the argument (see item 24).
	//	Rule: If expr is an lvalue, both T and ParamType are deduced to be lvalue references (this is the only situtation where T is deduced to be a reference). If expr is an rvalue: If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T (case 1 rules)
	//	void f_iv(T&&)
	f_iv(x);					//	x is lvalue, T = int&, ParamType = int&
	f_iv(cx);					//	cx is lvalue, T = const int&, ParamType = const int&
	f_iv(rx);					//	rx is lvalue, T = const int&, ParamType = const int&
	f_iv(27);					//	27 is rvalue, T = int, ParamType = int&& 	<(not-const?)>
	f_iv(*px);					//	*px is lvalue, 
	//	Ongoing: 2022-02-20T02:45:40AEDT (about the case of the const forwarding reference?)
	//	void f_v(const T&&)	
	//f_v(x);					//	invalid			<(x is lvalue, <?>)>
	//f_v(cx);					//	invalid 		<(cx is lvalue, <?>)>
	//f_v(rx);					//	invalid			<(rx is lvalue, <?>)>
	f_v(27);					//	27 is rvalue, T = int, ParamType = const int&&
	//f_v(*px);					//	invalid			<?>
	cout << "\n";


	//	Case 3)	ParamType is neither a pointer nor reference (pass by value), 'param' will be a copy of whatever is passed in, which is modifiable even if the origional value is const (type decay).
	//	Rule: If expr's type is a reference, ignore the reference part. If expr is const or volatile, ignore that too.
	//	void f_vi(T)
	f_vi(x);					//	T = int, ParamType = int
	f_vi(cx);					//	T = int, ParamType = int
	f_vi(rx);					//	T = int, ParamType = int
	f_vi(27);					//	T = int, ParamType = int
	f_vi(*px);					//	T = int, ParamType = int
	//	void f_vii(const T)
	f_vii(x);					//	T = int, ParamType = const int
	f_vii(cx);					//	T = int, ParamType = const int
	f_vii(rx);					//	T = int, ParamType = const int
	f_vii(27);					//	T = int, ParamType = const int
	f_vii(*px);					//	T = int, ParamType = const int
	cout << "\n";


	//	Array types versus Pointer types: are different, but they sometimes seem interchangable since in many contexts an array decays into a pointer to its first element.
	const char name[] = "J. P. Briggs";
	const char* pname = name;					//	array decays to pointer

	//	The following are equivalent: 
	void arrFunc_i(int param[]);
	void arrFunc_ii(int* param);

	//	Example: passing an array vs pointer to function template by-value, (array decays to pointer)
	//	void f_vi(T)
	f_vi(name);					//	T = const char*
	f_vi(pname);				//	T = const char*
	cout << "\n";

	//	Example: passing an array vs pointer to function template by-reference (references to arrays do not decay, the type deduced includes the type and size of the array).
	//	void f_i(T&)
	//	Ongoing: 2022-02-21T03:47:25AEDT (see below) the YCM deduced types for f_i:
	f_i(name);					//	T = const char [13], ParamType = const char (&)[13]
	f_i(pname);					//	T = const char*, ParamType = const char* 	<(YCM: ParamType = const char*&)>
	cout << "\n";

	//	Demonstate compileTimeArraySize() compile-time array-size 
	cout << "compileTimeArraySize(name)=(" << compileTimeArraySize(name) << ")\n";
	cout << "\n";

	//	<(Example: passing an array-vs-pointer to function template by forwarding reference)>


	//	Example: function type decay into function pointers
	//		void someFunc(int, double);
	//		void f1(T param);
	//		void f2(T& param);
	f1(someFunc);		//	T = void (*)(int,double), ParamType = ptr-to-func 		
	f2(someFunc);		//	T = void (&)(int,double), ParamType = ref-to-func 		
	//	Types according to YCM:
	//		template <> void f1<void (*)(int, double)>(void (*param)(int, double))
	//		template <> void f2<void    (int, double)>(void (&param)(int, double))
	cout << "\n";

	//	Example: type decay (case 3), calling template function that takes by-value argument with 'const int&' yields deduced type 'int' (consider, a reference acts like the thing it refers to). (Avoid type decay by using reference parameter types).
	int x1 = 5;
	const int& cx1 = x1;
	f1(cx1);			//	T = int, ParamType = int

	return 0;
}

//	Summary:
//		During template type-deduction, arguments that are references are treated as non-references. <names/types> that are arrays or functions decay to pointers unless they are used to initalize references
//		Case 1) pointer/non-forwarding-reference: ignore 'expr's reference-ness, then pattern match expr against ParamType to determine T. 
//		Case 2) forwarding-reference: expr is an lvalue, T and ParamType are deduced to be references (only case where T is a reference). Otherwise apply case 1 rules. 
//		Case 3) pass-by-value, ignore 'expr's reference-ness, and const/volatile-ness.

