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
struct Widget {
	double z;
};

//	'decltype()' gives the type of an expression or entity (think 'typeof()'). Result includes reference and cv specifiers.

//	'declval<T>()' returns an rvalue reference to whatever is passed to it
//	<(creates dummy values for use with 'decltype'?)>
//	Return type is 'T&&', unless T is <(possibly cv-qualified(?))> void, in which case return type is 'T'


int f(int x) { cout << "int(x)=(" << x << ")->int\n"; return 0; }
char f(bool x) { cout << "bool(x)=(" << x << ")->char\n"; return 0; }
double f(char x) { cout << "char(x)=(" << x << ")->double\n"; return 0; }


//	Example: use 'declval' for a conditional 'decltype' -> the type resulting from the addition of types T/U
template<typename T, typename U>
using sum_t = decltype(std::declval<T>() + std::declval<U>());


int main(int argc, char* argv[])
{
	//	Get the return type of various overloads
	cout << get_type_name<decltype(f(declval<int>()))>() << "\n";
	cout << get_type_name<decltype(f(declval<bool>()))>() << "\n";
	cout << get_type_name<decltype(f(declval<char>()))>() << "\n";
	cout << "\n";

	//	Get the type of a class member
	cout << get_type_name<decltype(declval<Widget>().z)>() << "\n";
	cout << "\n";

	//	Get the noexcept status of copy/move ctor
	//	implicit ctor's are noexcept <(where one would expect them to be noexcept)>
	cout << std::boolalpha << "noexcept(Widget(copy))=(" << noexcept(Widget(std::declval<Widget&>())) << ")\n";
	cout << std::boolalpha << "noexcept(Widget(move))=(" << noexcept(Widget(std::declval<Widget&&>())) << ")\n";
	cout << "\n";

	//	Ongoing: 2022-04-01T18:08:44AEDT Ycm does not produce error, and compiler error is 'decltype' is an undefined symbol
	//f(declval<int>());				//	error, cannot declare in an evaluated context

	return 0;
}

