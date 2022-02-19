#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	C++98 has a single set of rules for type deduction: those for function templates. C++11 adds rules for auto, and rules for decltype. C++14 extends the contexts in which these later two may be employed.
//	Type deduction can make the types of variables in codes not as apparent as one might like.

//	A function template has the form:
//			template<typename T>
//			void f(ParamType x);
//	And is called:
//			f(expr);
//	During compilation, expr is used to deduce two types: one for T, and one for ParamType

//	For example: 
//		template<typename T>
//		void f(const T& param) { cout << "param=(" << param << ")\n"; }
//	When 'f(x)' is called for 'int x = 0', ParamType is deduced to be 'const T&' and T is deduced to be 'int'.

//	TODO: 2022-02-20T02:36:19AEDT effective-c++, item 01, template-type-deduction, descriptive names for f_.* functions

//	Case 1:
template<typename T> void f_i(T& param) { cout << "f_i(" << param << ")\n"; }
template<typename T> void f_ii(const T& param) { cout << "f_ii(" << param << ")\n"; }
template<typename T> void f_iii(T* param) { cout << "f_iii(" << *param << ")\n"; }
//	Ongoing: 2022-02-20T01:53:30AEDT <the-case-of> const-pointer and pointer-to-const

//	Case 2:
template<typename T> void f_iv(T&& param) { cout << "f_iv(" << param << ")\n"; }
//	Ongoing: 2022-02-20T01:29:32AEDT (about <the-case-of>) const universal reference?
template<typename T> void f_v(const T&& param) { cout << "f_v(" << param << ")\n"; }

//	Case 3:
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

	//	Case 1: ParamType is a pointer, or a non-universal (non-forwarding) reference
	//	If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T
	int x = 27;					//	int
	const int cx = x;			//	const int
	const int& rx = x;			//	reference to const int
	const int* px = &x;			//	pointer to const int
	const int* const cpx = &x;	//	const pointer to const int
	//	void f_i(T&)
	//	Passing a const object to a template taking a 'T&' parameter is safe, because the const-ness of the object is deduced as part of type 'T'
	f_i(x);						//	T = int, ParamType = int&
	f_i(cx);					//	T = const int, ParamType = const int&
	f_i(rx);					//	T = const int, ParamType = const int&
	//f_i(27);					//	invalid
	//	void f_ii(const T&)
	f_ii(x);					//	T = int, ParamType = const int&
	f_ii(cx);					//	T = int, ParamType = const int&
	f_ii(rx);					//	T = int, ParamType = const int&
	f_ii(*px);					//	<?>
	f_ii(27);					//	<?>
	//	void f_iii(T*)
	f_iii(&x);					//	T = int, ParamType = int*
	f_iii(px);					//	T = const int, ParamType = const int*
	f_iii(cpx);					//	<?>
	//f_iii(27);				//	invalid
	cout << "\n";


	//	Case 2: ParamType is a universal (forwarding) reference, preserves the lvalue/rvalue-ness of the argument (see item 24).
	//	If expr is an lvalue, both T and ParamType are deduced to be lvalue references (this is the only situtation where T is deduced to be a reference).
	//	If expr is an rvalue: If expr's type is a reference, ignore the reference part. Then pattern match expr's type against ParamType to determine T (that is, Case 1 rules apply)
	//	void f_iv(T&&)
	f_iv(x);					//	x is lvalue, T = int&, ParamType = int&
	f_iv(cx);					//	cx is lvalue, T = const int&, ParamType = const int&
	f_iv(rx);					//	rx is lvalue, T = const int&, ParamType = const int&
	f_iv(27);					//	27 is rvalue, T = int, ParamType = int&& 	<(not-const?)>
	f_iv(*px);					//	<?>
	//	Ongoing: 2022-02-20T02:45:40AEDT (about the case of the const forwarding reference?)
	////	void f_v(const T&&)	
	//f_v(x);						//	x is lvalue, <?>
	//f_v(cx);					//	cx is lvalue, <?>
	//f_v(rx);					//	rx is lvalue, <?>
	//f_v(27);					//	27 is rvalue, <?>
	//f_v(*px);					//	<?>
	cout << "\n";


	//	Case 3:	ParamType is neither a pointer nor reference (pass by value), 'param' will be a copy of whatever is passed in, which is modifiable even if the origional value is const.
	//	If expr's type is a reference, ignore the reference part. If expr is const or volatile, ignore that too.
	//	void f_vi(T)
	f_vi(x);					//	T = int, ParamType = int
	f_vi(cx);					//	T = int, ParamType = int
	f_vi(rx);					//	T = int, ParamType = int
	f_vi(27);					//	<?>
	f_vi(*px);					//	<?>
	//	void f_vii(const T)
	f_vii(x);					//	<?>
	f_vii(cx);					//	<?>
	f_vii(rx);					//	<?>
	f_vii(27);					//	<?>
	f_vii(*px);					//	<?>
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
	f_i(name);					//	T = const char [13], ParamType = const char (&)[13]
	f_i(pname);					//	T = <?>
	cout << "\n";

	//	Demonstate compileTimeArraySize() compile-time array-size 
	cout << "compileTimeArraySize(name)=(" << compileTimeArraySize(name) << ")\n";
	cout << "\n";

	//	<(Example: passing an array-vs-pointer to function template by forwarding reference)>


	//	Example: function type decay into function pointers
	//		void someFunc(int, double);
	//		void f1(T param);
	//		void f2(T& param);
	f1(someFunc);		//	T = void (*)(int,double), ParamType = ptr-to-func 		<?>
	f2(someFunc);		//	T = void (&)(int,double), ParamType = ref-to-func 		<?>

	cout << "\n";

	return 0;
}

//	Summary:
//		During template type-deduction, arguments that are references are treated as non-references
//		When deducing types for forwarding reference parameters, lvalue arguments get special treatment
//		When deducing types for by-value parameters, const/volatile are ignored
//		During template type-deduction, for non-reference arguments, <names/types> that are arrays or functions decay to pointers

