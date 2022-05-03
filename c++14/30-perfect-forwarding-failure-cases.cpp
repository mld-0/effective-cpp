//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	is_lvalue(T&&), get_type_name<T>()
//	{{{
//	'is_lvalue()', determine is argument is lvalue/rvalue 
template<typename T>
constexpr bool is_lvalue(T&& x) {
//	{{{
	return std::is_lvalue_reference<T>{};
}
//	}}}
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
//	}}}
class Widget {};

//	Perfect forwarding is where a function passes an object to a second function so that it is recieved exactly as if that second function had been called directly, preserving the lvalue/rvalue-ness of the object along with any const/volatile qualifiers. In C++, perfect forwarding is achieved through 'std::forward<T>()' and forwarding references.
//	Forwarding references are, by definition, rvalue references used with type deduction.


//	Example: A failure of perfect forwarding is where 'fwd_pd(expr)' yields a different result to 'print_details(expr)'
template<typename T>
void fwd_pd(T&& x) {
	print_details(std::forward<T>(x));
}
template<typename T>
void print_details(T&& x) {
	//	Ongoing: 2022-03-18T21:16:01AEDT here we have a use of forward without 'T' ... how is it working?
	//cout << "print_details: type(" << get_type_name<decltype(x)>() << "), is_lvalue(" << is_lvalue(forward(x)) << ")\n";
	cout << "print_details: type(" << get_type_name<decltype(x)>() << "), is_lvalue(" << is_lvalue(std::forward<T>(x)) << ")\n";
}
//	TODO: 2022-03-18T00:01:06AEDT effective-c++, item 30, perfect-forwarding-failure-cases, variadic 'fwd_v', call 'print_details' for each argument passed (but first, extra/variadic-templates)
//	Example: <(Do we need a variadic form of 'fwd_pd' to explain any of our failure-cases?)>
template<typename... Ts>
void fwd_v(Ts&&... params) {
}


//	{{{
//	Ongoing: 2022-03-18T00:13:53AEDT The <old/new-c++> way to say 'callable template parameter' (not that we necessarily need to - our use requires any <instansiating> type to be useable in the fashion 'f(x)' <(the point being ~~template-functions~~ perfect forwarding lead to poor error messages (presumedly refering to the callee,) (but also function templates probably have worse error messages that a 'static_assert(is_callable)' might?
//	}}}
//	Where 'fwd(expr, f)' is different from 'f(expr)' we have a failure of perfect forwarding
template<typename T, typename Callable>
decltype(auto) fwd(T&& x, Callable f) {
	cout << "fwd(" << get_type_name<decltype(x)>() << "," << get_type_name<Callable>() << "): ";
	return f(std::forward<T>(x));
}
//	(By-value equivalent:)
template<typename T, typename Callable>
decltype(auto) fwd_byvalue(T x, Callable f) {
	cout << "fwd_byvalue(" << get_type_name<decltype(x)>() << "," << get_type_name<Callable>() << "): ";
	return f(x);
}



//	Example: Cannot pass '0' (int) or 'NULL' (long) to a function expecting a pointer through a template function (taken from item 08)
template<typename FuncType, typename PtrType>
decltype(auto) call_func(PtrType&& ptr, FuncType func) {
	return func(std::forward<PtrType>(ptr));
}
int f1(shared_ptr<Widget> x) { cout << "f1(shared_pt<Widget>)\n"; return 2; }
bool f2(Widget* x) { cout << "f2(Widget*)\n"; return false; }


void f_vector(const vector<int>& v) { cout << "f_vector, v.size()=(" << v.size() << ")\n"; }

//	Ongoing: 2022-03-18T00:04:06AEDT perfect forwarding example: template which takes 'T&&' argument, and forwards it to function 'f' also passed as argument

int main()
{
	//	Failure case: Braced initalizer
	//	Here, with a braced initalizer list for 'expr', we have a perfect forwarding failure per our definition of 'fwd' above, because braced initalizer lists cannot be used with template functions.
	//	Ongoing: 2022-03-18T01:13:00AEDT (example for) failure of perfect forwarding due to deduction of wrong type(?) [...] -> our next item, 0/NULL?
	//	Ongoing: 2022-03-18T00:55:58AEDT (items on braced-initalizers/template-functions) (and do/should they talk about initaliser lists not being accepted by template functions)
	f_vector( {1,2,3} );
	//fwd( {1,2,3}, f_vector );									//	error, cannot infer type T

	//	(This problem with braced initalizer literals is not exclusive to forwarding references, we cannot call our equivalent by-value function this way either)
	//func_template( {1,2,3} );									//	error, cannot infer type T
	//fwd_byvalue( {1,2,3}, f_vector);							//	error, cannot infer type T

	//	We can, however, pass a literal initializer_list<int> both by forwarding-ref and by value:
	fwd(initializer_list<int>( {1,2,3} ), f_vector);
	fwd_byvalue(initializer_list<int>( {1,2,3} ), f_vector);
	cout << "\n";


	//	Failure case: '0' or 'NULL' as null pointers
	//	When 0 or NULL are passed to a template function, they are deduced as integral types (int and long respectively) (the later being system dependent) (see item 08). These parameters cannot be passed as pointers, even though the literal 0/NULL would have been accepted, a failure of perfect forwarding as per our definition of 'fwd' above.
	//	Ongoing: 2022-03-18T01:20:54AEDT again, 0/NULL are failure of perfect forwarding because they are failures of template functions(?)
	//fwd(0, f1);												//	error, cannot use deduced type 'int' as pointer
	//fwd(NULL, f1);											//	error, cannot use deduced type 'long' as pointer
	fwd(nullptr, f1);
	//fwd(0, f2);												//	error, cannot use deduced type 'int' as pointer
	//fwd(NULL, f2);											//	error, cannot use deduced type 'long' as pointer
	fwd(nullptr, f2);
	cout << "\n";
	//	note: calling f1/f2 with 0/NULL is valid
	f1(0); 		f1(NULL);		f1(nullptr);
	f2(0); 		f2(NULL);		f2(nullptr);
	cout << "\n";


	//	Failure case: Declaration-only integral static-const data members
	//	Ongoing: 2022-03-18T01:36:09AEDT is there some assumed knowledge on static const class data members required for this section? Whether or not they are replaced with their value at compile time and/or they exist in memory with an address to be taken [...] and whether use of such a value as a declaration (including the value) (inside the class definition) <>
	//	Ongoing: 2022-03-18T01:39:36AEDT Declaring a variable inside a class (even if we are providing a value (or is that only true for static-and/or-const variables)) is a declaration not a definition?
	//	<>


	//	Failure case: Overloaded function names and template names
	//	<>


	//	Failure case: Bitfields
	//	<>


	return 0;
}

//	Summary:
//		<>

