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

//	TODO: 2022-03-13T22:54:39AEDT effective-c++, item 03, decltype, f----- behaviour of functions called inside 'decltype()', an explanation to go with the 'update_counter()' side-effects example

//	For Vim with YCM, check deduced types with
//			:YcmCompleter GetType


//	decltype <C++11> Inspects the declared type of an entity, or the type and value category of an expression.
//	It is useful when declaring types that are difficult or impossible to declare using standard notation (lambda-related types, or types that depend on template parameters).
//	The primary use for decltype is declaring function templates whose return type depends on its parameter types


//	Pitfall: If the name of an object is parenthesized, it is treated as an ordinary lvalue expression, therefore:
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
decltype(foo()) && r1 = foo();		//	T = int&&	<(YCM says T = int?>
decltype((n)) && r2 = n;			//	T = int& 	('& &&' collapses into '&')

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
decltype(v[0]) var6 = v[0];			//	T = int& 	<(the ugly truth is '__vector_base<int, std::allocator<int>>::value_type &'? which collapses into 'int&'?)>


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


//	Example: decltype(auto) return type and parenthesis, note that 'x' and '(x)' are deduced as different types.
decltype(auto) f1() {
	int x = 0;
	return x;		//	return type = int
}
//	Ongoing: 2022-03-13T23:01:12AEDT 'f2', some way to silence the warning where 'return (x)' is deduced to be int&
decltype(auto) f2() {
	int x = 0;
	return (x);		//	warning, return type = int&
}

//	Ongoing: 2022-02-21T03:12:41AEDT an auto/decltype(auto) template function (comperable to accessElement as-a-trick) and having a YCM neat correct deducable type -> for a template function, the type to get is for the called insubstantitation(?)


//	Example: If the name we pass to decltype is one of a variable defined in structured binding, then the result is the type of the bound-to element
//	LINK: https://arne-mertz.de/2017/01/decltype-declval/
//	{{{
//	If what we pass to decltype is the name of a variable (e.g. decltype(x) above) or function or denotes a member of an object (decltype x.i), then the result is the type of whatever this refers to. As the example of decltype(y) above shows, this includes reference, const and volatile specifiers.
//	
//	An exception to this last rule is the use of C++17’s structured binding: If the name we pass to decltype is one of a variable defined in structured binding, then the result is the type of the bound-to element. Example:
//	
//	std::pair<int volatile &&, double&> f(int);
//	auto const& [a, b] = f(22);
//	While the type of a is int const volatile&, decltype(a) will give int volatile&&, as that is the type of the first element of f‘s return value. Similarly, decltype(b) will result in double&, not double const&.
//	
//	If the expression passed to decltype is not just a name or member access expression, the resulting type depends on the value category of the expression. Given the type of the expression e is E, then decltype(e) is
//	
//	E, if e is an prvalue,
//		E&, if e is an lvalue, and
//		E&&, if e is a xvalue
//		As an example, the above decltype(&X::bar) is just a member function pointer and not a reference to one, because the built-in address-of operator returns a prvalue.
//	
//		These rules may look complicated, but they mostly do what you’d naturally expect, with the exceptions of the mentioned results of structured binding and the fact that a name expression in parentheses makes it an lvalue. That means, that when x is a variable of type X, then decltype((x)) will give X& as opposed to decltype(x) giving x.
//	}}}


int main()
{
	//	Example: 'decltype()' does not execute anything, does not produce side effects
	cout << get_type_name<decltype(std::cout << "Does not print\n")>() << "\n";		//	std::ostream&
	int counter = 0;
	cout << get_type_name<decltype(++counter)>() << "\n";							//	int&
	cout << "counter=(" << counter << ")\n";										//	0

	//	space between compile warning and output for vim 'Exe'
	cout << "\n";	

	vector<int> v1 = {1,23,4,7};

	//accessElement_i(v1, 1) = 1;			//	error, deduced return type is 'int'

	//	Ongoing: 2022-02-21T03:15:13AEDT Collapsing the YCM/IDE deduced type for 'accessElement_.*' to a neat <correct> 'int&' (or is it int&&'?) (from the 

	accessElement_ii(v1, 1) = 2;			
	//	T = template <> int &accessElement_ii<std::vector<int> &, int>(std::vector<int> &c, int i)

	accessElement_iii(v1, 1) = 3;
	//	T = template <> auto accessElement_iii<std::vector<int> &, int>(std::vector<int> &c, int i) -> decltype(std::forward<std::vector<int> &>(c)[i])


	//	Ongoing: 2022-02-21T03:21:21AEDT (much about) (see below) (unnecessariy specification of template parameters)
	//accessElement_ii<vector<int>&, vector<int>::size_type>(v1,2);

	f1();
	f2();


	//	Example: f----- behaviour of functions called inside 'decltype()'
	int side_effect_counter = 0;
	auto update_counter = [&side_effect_counter]() { ++side_effect_counter; return 0; };
	cout << "side_effect_counter=(" << side_effect_counter << ")\n";
	decltype(update_counter()) vuc1;
	cout << "decltype(update_counter())\n";
	cout << "side_effect_counter=(" << side_effect_counter << ")\n";
	update_counter();
	cout << "update_counter()\n";
	cout << "side_effect_counter=(" << side_effect_counter << ")\n";


	(void) var1; (void) vuc1;
	return 0;
}

//	Summary:
//		For a variable of type T, decltype return T given the variable name, T& given an lvalue, and T given an rvalue. (Note that 'x' is a name, whereas '(x)' is an lvalue).
//		decltype(auto) is like 'auto', but performs type deduction using decltype rules

