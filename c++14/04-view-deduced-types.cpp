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

//	TODO: 2022-03-08T05:03:04AEDT effective-c++, item 04, view deduced types, version of 'get_type_name<T>()' which doesn't produce YCM warning/error

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

//	Ongoing: 2022-02-25T00:44:04AEDT examples of C++ types to be simplified (by tool to be found) (see below)
//	{{{
//		__vector_base<int, std::allocator<int>>::value_type & 	->	int&	<(doesn't work (for get_type_name<T>()))>
//		vector<int>::reference	->	int&
//		basic_string<char, char_traits<char>, allocator<char>>	->	string
//	}}}

//	For Vim with YCM, check deduced types with
//			:YcmCompleter GetType

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

//	compiler based diagnostics - making the compiler 
//	Example: 'TD' Type Deduction, (compiler based diagnostics), a template class purpously lacking a definition, when insubstantiated as 'TD<decltype(var)>', the resulting compiler error message provides the type of 'var'.
template<typename T>
class TD;


//	Ongoing: 2022-03-08T05:16:50AEDT should not 'a'/'b' be forwarded for 'make_pair_byforwardingref'? (meaning of move for the std::pair ctor) <(book does not mention here, has not yet examined move/forward?)>
//	Example: arrays passed by reference do not decay, length becomes part of type
template<typename T>
auto make_pair_byvalue(T a, T b) {
	return std::pair<T,T>(a, b);
}
template<typename T>
auto make_pair_byref(T& a, T& b) {
	return std::pair<T,T>(a, b);
}
template<typename T>
auto make_pair_byforwardingref(T&& a, T&& b) {
	return std::pair<T,T>(a, b);
}
//	Note that the type of parameter is uniform <(can not all three cases be handled by the forwarding reference one?)>, <(findings from examining cases 'f(T&&,T&)', 'f(T&&,T)', ect)>


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
	//	Result is compiler dependent							//	g++/clang output:
	cout << typeid(x).name() << "\n";							//	int				i		
	cout << typeid(y).name() << "\n";							//	int const*		PKi
	cout << typeid(w1).name() << "\n";							//	Widget			6Widget
	cout << typeid(pw1).name() << "\n";							//	Widget*			P6Widget
	cout << "\n";
	
	
	
	

	const auto vw = createWidgetVec();
	if (!vw.empty()) {
		foo(&vw[0]);
	}
	cout << "\n";
	//	T = PK6Widget
	//	param = PK6Widget

	cout << get_type_name<decltype(x)>() << "\n";				//	int
	cout << get_type_name<decltype(y)>() << "\n";				//	const int *
	cout << get_type_name<decltype(w1)>() << "\n";				//	Widget
	cout << get_type_name<decltype(pw1)>() << "\n";				//	Widget *
	cout << "\n";
	

	cout << get_type_name<vector<int>::reference>() << "\n";
	cout << get_type_name<basic_string<char, char_traits<char>, allocator<char>>>() << "\n";
	cout << "\n";
	//	int &
	//	std::string

	const char carr1[] = "abc";
	const char carr2[] = "hijklmnop";

	//	Example: passing by value, array type decay, can pass different length arrays as same type
	make_pair_byvalue(carr1, carr2);

	//	Ongoing: 2022-03-08T05:12:25AEDT TO-DO (item <?>) is the cases <of/like> 'f(T&&, T&)' and 'f(T&&, T)' <(add details of examination of said cases)>

	//	Example: passing by reference, length is part of deduced type, arrays of same type must be same length
	//make_pair_byref(carr1, carr2);							//	error, conflicting types 
	//make_pair_byforwardingref(carr1, carr2);					//	error, conflicting types
	//make_pair_byforwardingref(move(carr1), move(carr2));		//	error, conflicting types


	//	TODO: 2022-02-25T01:07:35AEDT effective-c++, item 04, view-deduced-types, complete chapter/item

	return 0;
}

