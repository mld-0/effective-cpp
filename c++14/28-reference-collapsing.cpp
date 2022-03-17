//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	get_type_name<T>()
//	{{{
//	LINK: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#include <string_view>
template <typename T>
constexpr auto get_type_name() -> std::string_view {
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
//	is_lvalue()
//	{{{
template<typename T>
constexpr bool is_lvalue(T&& x) {
	return std::is_lvalue_reference<T>{};
}
//	}}}

//	Reference Collapsing:
//	Declaring a reference to a reference is normally illegal in C++. However, the compiler may create such chained references, upon which reference collapsing is performed. 
//	The rules for reference collapsing are: If either reference is an lvalue reference, the result is an lvalue reference, otherwise the result is an rvalue reference.
//			A& &		A&
//			A&& &		A&
//			A& &&		A&
//			A&& &&		A&&

//	Reference collapsing occurs in four scenarios: 1) template instantiation, 2) auto variables, 3) generation and use of typedef and alias declarations, and 4) during analysis by 'decltype()'

//	Ongoing: 2022-03-13T22:06:53AEDT (TO-DO is determining T for a lambda with auto parameters for an equivalent template)

//	Forwarding references are only possible where type deduction is used, because deduction of type according to lvalue/rvalue-ness of argument, combined with reference collapsing, is the mechanism through which forwarding references work.
//	Reference collapsing is the mechanism through which a forwarding reference can act as either lvalue or rvalue. When an argument is passed to a forwarding reference function parameter, the type deduced for T encodes whether the argument recieved was an lvalue/rvalue:
//	When an lvalue is passed as argument, T is deduced to be an lvalue reference (T&):
//			func_i(Widget& && param)	->		func_i(Widget& param)
//	When an rvalue is passed as argument, T is deduced to be a non-reference (T):
//			func_i(Widget&& param)
template<typename T>
void func_i(T&& param) {}


//	Ongoing: 2022-03-13T22:12:12AEDT 'typename std::remove_reference<T>::type&&' is [...] not(?) considered type deduction for the purposes of [...] forwarding references -> necessitating the declaration of two versions.
//	Approximation of 'std::forward<T>()'. Performs a <static_cast> to T&&. Implementation requires versions for lvalue and rvalue <>
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type& t) noexcept {
	cout << "my_forward(&)\n";
    return static_cast<T&&>(t);
}
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type&& t) noexcept {
	cout << "my_forward(&&)\n";
    static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}
//	When given an rvalue, T is deduced as a non-reference, and the result is a cast to T && = T&&
//	When given an lvalue, T is deduced as T&, and the result is a cast to T& && = T&

//	Widget:
class Widget {};
Widget createWidget() { return Widget(); }

//	Example: 'decltype(x)' and 'decltype(forward<T>(x))' produce the same type
template<typename T>
void compare_type_forward_noforward(T&& x) {
	cout << "type(x): " << get_type_name<decltype(x)>() << ", ";
	cout << "type(forward<T>(x)): " << get_type_name<decltype(forward<T>(x))>() << "\n";
}


void recieve_Widget(Widget& x) { cout << "recieve_Widget(&)"; }
void recieve_Widget(Widget&& x) { cout << "recieve_Widget(&&)"; }


//	TODO: 2022-03-17T20:15:47AEDT effective-c++, item 28, reference-collapsing, examples for 'typedef and alias declarations' and 'decltype'
//	Example(s): Reference collapsing for typedef and alias declarations
//	<>
//	Example(s): Reference collapsing for decltype
//	<>


//	Example: <('decltype(x)(x)' vs 'forward<decltype(x)>(x)'?)>
template<typename T>
void compare_forward_alternatives(T&& x) {
	//	Note that T != decltype(x), (even though both work for the purpouses of 'std::forward')
	cout << "forward<T>(x): ";
	cout << get_type_name<decltype(forward<T>(x))>() << ", ";
	recieve_Widget(forward<T>(x));
	cout << "\n";
	cout << "forward<decltype(x)>(x): ";
	cout << get_type_name<decltype(forward<decltype(x)>(x))>() << ", ";
	recieve_Widget(forward<decltype(x)>(x));
	cout << "\n";
	cout << "decltype(x)(x): ";
	cout << get_type_name<decltype(decltype(x)(x))>() << ", ";
	recieve_Widget(decltype(x)(x));
	cout << "\n";
	cout << "x: ";
	cout << get_type_name<decltype(x)>() << ", ";
	recieve_Widget(x);
	cout << "\n";
}
//	Ongoing: 2022-03-14T23:14:57AEDT some authority <agreeing> on the use of 'decltype(x)(x)' <- this <>


//	Ongoing: 2022-03-13T23:04:16AEDT reference collapsing gives us the reason 'forward<T>(x)' and forward<decltype(x)>(x)' are (both) valid -> [...] <(determine both are correct for lvalue/rvalue-reference cases [...] working through the reference collapsing and the casts?)> 
int main()
{
	Widget w1;
	string s1 = "asdf";

	func_i(w1);						//	T = Widget&, ParamType = Widget& && = Widget&
	func_i(createWidget());			//	T = Widget, ParamType = Widget&&

	//compare_type_forward_noforward(w1);
	//compare_type_forward_noforward(move(w1));
	//cout << "\n";

	//	Ongoing: 2022-03-14T01:22:15AEDT does the T/ParamType terminology (T='auto', ParamType='auto&&') for describing 'auto&&' have some official basis / is there precise C++ standards language for describing 'auto'-in-auto-variables?
	//	Ongoing: 2022-03-14T01:25:59AEDT (this explains YCM's output: for 'auto&& aw1 = w1', auto = Widget&, and for 'auto&& aw2 = createWidget()', auto = Widget) (there does not seem to be a way to get 'ParamType'?

	//	Example: reference collapsing and auto variables
	auto&& aw1 = w1;					//	T = Widget&, ParamType = Widget& && = Widget&
	//Widget& && rcaw1 = w1;			//	error, illegal reference to reference
	cout << "type(aw1)=(" << get_type_name<decltype(aw1)>() << ")\n";

	auto&& aw2 = createWidget();		//	T = Widget, ParamType = Widget&&
	Widget&& rcaw2 = createWidget();
	cout << "type(aw2)=(" << get_type_name<decltype(aw2)>() << ")\n";
	cout << "\n";

	compare_forward_alternatives(move(w1));
	cout << "\n";

	(void) rcaw2;
	return 0;
}

//	Summary:
//		Reference collapsing occurs in four contexts: 1) template instantiation, 2) auto type generation, 3) creation and use of typedefs/alias-declarations, 4) decltype
//		It is illegal for the user to declare references to references, but the compiler may do so, in which case reference collapsing occurs. If both references are rvalue references, the result is an rvalue reference, otherwise it is an lvalue reference.
//		Forwarding references are rvalue references in context where type deduction occurs, as type deduction combined with reference collapsing is the mechanism through which forwarding references work. 

