//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	TODO: 2022-02-25T00:46:48AEDT effective-c++, item 04, view-deduced-types, use of Boost TypeIndex library

//	For Vim with YCM, check deduced types with
//			:YcmCompleter GetType

//	Ongoing: 2022-02-25T00:44:04AEDT examples of C++ types to be simplified (by tool to be found) (see below)
//	{{{
//		__vector_base<int, std::allocator<int>>::value_type & 	->	int&	<(doesn't work (for get_type_name<T>()))>
//		vector<int>::reference	->	int&
//		basic_string<char, char_traits<char>, allocator<char>>	->	string
//	}}}

class Widget {};

vector<Widget> createWidgetVec() { 
	return { Widget{} }; 
}

template<typename T>
void foo(const T& param) { 
	cout << "foo(), param=(" << param << ")\n"; 
	cout << "T = " << typeid(T).name() << "\n";
	cout << "param = " << typeid(param).name() << "\n";
} 

//	TODO: 2022-02-21T03:24:06AEDT effective-c++, item 04, view-deduced-types, a tool to <turn/reduce> '__vector_base<int, std::allocator<int>>::value_type &' into 'int&' (c++filt -n <> for demangling of objdump symbols), ()


//	Compiler Diagnostics: Making the compiler show a type it has deduced by using it erroneously.
//	Example: 'TD' Type Deduction, a template class purpously lacking a definition, provides error message featuring deduced type T, when insubstantiated for 'decltype(var)', the resulting error message provides the type of 'var'.
template<typename T>
class TD;



//	Example: get_type_name<T>()
//	LINK: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#include <string_view>
template <typename T>
constexpr auto get_type_name() -> std::string_view
{
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


int main()
{
	const int theAnswer = 42;
	auto x = theAnswer;
	auto y = &theAnswer;

	//	Use of Type Deduction template
	//TD<decltype(x)> xType; 	//	error, Implicit instantiation of undefined template 'TD<int>'
	//TD<decltype(y)> yType; 	//	error, Implicit instantiation of undefined template 'TD<const int *>'

	Widget w1;
	Widget* pw1 = &w1;

	//	Runtime type information: 'typeid()' and 'std::typeinfo::name'
	//	Result is compiler dependent
	cout << typeid(x).name() << "\n";
	cout << typeid(y).name() << "\n";
	cout << typeid(w1).name() << "\n";
	cout << typeid(pw1).name() << "\n";
	cout << "\n";
	//	g++/clang output:
	//	int				i		
	//	int const*		PKi
	//	Widget			6Widget
	//	Widget*			P6Widget

	const auto vw = createWidgetVec();
	if (!vw.empty()) {
		foo(&vw[0]);
	}
	cout << "\n";
	//	T = PK6Widget
	//	param = PK6Widget

	cout << get_type_name<decltype(x)>() << "\n";
	cout << get_type_name<decltype(y)>() << "\n";
	cout << get_type_name<decltype(w1)>() << "\n";
	cout << get_type_name<decltype(pw1)>() << "\n";
	cout << "\n";
	//	int
	//	const int *
	//	Widget
	//	Widget *

	cout << get_type_name<vector<int>::reference>() << "\n";
	cout << get_type_name<basic_string<char, char_traits<char>, allocator<char>>>() << "\n";

	return 0;
}

