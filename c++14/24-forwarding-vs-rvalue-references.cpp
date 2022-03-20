//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;
class Widget {};
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

//	LINK: http://thbecker.net/articles/rvalue_references/section_01.html

//	Ongoing: 2022-02-26T02:06:45AEDT should this not be covered before move/forward (item 23)
//	<(Here we will make a simplifications about '&&'?)>

//	'T&&' has two meanings. It can mean rvalue reference, or it can mean forwarding reference.
//	A forwarding reference is an rvalue reference to a cv-unqualified template parameter. A forwarding reference can behave as either an lvalue reference 'T&' or an rvalue reference 'T&&'. 
//	<(bind to const, bind to volatile)>.
//	If 'T&&' is used in a context without type deduction, it is an rvalue reference not a forwarding reference.

//	Forwarding references, (like all references) must be initialized.
//		If a forwarding reference is initalized with an lvalue, it acts as an lvalue reference
//		If a forwarding reference is initalized with an rvalue, it acts as an rvalue reference

//	Functions using forwarding reference tend to produce very verbose error messages when they are used with the wrong type. The more layers of perfect forwarding, the more verbose the error message becomes.

//	Example: determine is argument is lvalue/rvalue 
template<typename T>
constexpr bool is_lvalue(T&& x) {
	//return std::is_lvalue_reference<T>();
	//return std::is_lvalue_reference<decltype(x)>();
	return std::is_lvalue_reference<decltype(forward<T>(x))>();
}
//	Ongoings:
//	{{{
//	Ongoing: 2022-03-02T02:12:02AEDT <(I <dispute/question/fail-to-see-how> how 'is_lvalue(forward(x))' gives the same result for 'int&&' and 'int' arguments of 'x')> -> because we are using 'T'? -> replace T with decltype(forward<T>(x)) gives same result [...] why are we using '{}' instead of '()'? -> it is all valid, and our mistake is using 'forward<>' outside a function taking a forwarding reference?
//	Ongoing: 2022-03-13T00:06:00AEDT re, the meaning of T in forward<T> -> consider 'is_lvalue_reference<T>' vs 'is_lvalue_reference<decltype(x)>' vs 'is_lvalue_reference<decltype(forward<T>(x))>'?
//	}}}


//	Example: type deduction of an rvalue-reference, declares a forwarding reference
template<typename T> void f_iv(T&& param);
auto&& var3 = Widget();

//	Example: no type deduction, declares an rvalue reference
void f_v(Widget&& param);
Widget&& var4 = Widget();

//	Example: type deduction is necessary but not sufficient for '&&' to be a forwarding reference
template<typename T>
void f_vi(vector<T>&& param) {}			// param is an rvalue reference (vector<T>)
template<typename T>
void f_vii(const T&& param) {}			// param is an rvalue reference (const)


//	Ongoing: 2022-03-02T02:08:18AEDT (after completing extra/variadic-templates), implement 'emplace_back' example
//	Example: template classes and forwarding references
//	Once my_vector<Widget> is instantiated, first function becomes 'push_back(Widget&& param)', type deduction is no longer involved and therefore 'param' is an rvalue reference. However, 'emplace_back(Args&&... args)' does still employ type deduction after instantiation, hence 'args' is <eligible to be> a forwarding reference.
template<class T, class Allocator = allocator<T>>
class my_vector {
	void push_back(T&& param);				//	param is an rvalue reference, not a forwarding reference
	template<class... Args>
	void emplace_back(Args&&... args);		//	args is a forwarding reference
};
//	Ongoing: 2022-02-26T02:28:53AEDT how this 'class... Args' parameter-pack example works
//	Ongoing: 2022-02-26T02:26:44AEDT how this rule (see above) can be better expressed

//	Ongoing: 2022-02-26T22:26:55AEDT does reference collapsing occur for rvalue references? (if not why not)

//	Example: __PRETTY_FUNCTION__
template<typename T>
void f(T&& param) {
	cout<< __PRETTY_FUNCTION__ << "\n";
}

//	Example: <('decltype(value)(value)' is an alternative to 'forward<decltype(value)>(value)')>


//	Reference Collapsing: (see item 28)
//		A& &		A&
//		A&& &		A&
//		A& &&		A&
//		A&& &&		A&&


//	Ongoing: 2022-03-16T00:23:19AEDT meaning of a const rvalue/forwarding reference?
//	Ongoing: 2022-03-16T00:03:11AEDT Avoid overloading forwarding references -> also avoid overloading rvalue references too?
//	Ongoing: 2022-03-15T22:55:20AEDT how to call rvalue-reference overload without using 'move'?
//	Example: lvalue/rvalue reference overloading rules
//	Rvalue references allow a function to branch at compile type on the question: "Am I being called on an lvalue or an rvalue" (Note, when passing an rvalue reference that is an lvalue, the lvalue reference overload is chosen - whether the lvalue/rvalue-reference overload is chosen depends not on the results of 'get_type_name<T>()', but of 'is_lvalue()'
//	<(Which version is called depends not on the type of the argument, but whether it is an lvalue/rvalue - for, 'Widget&& w3 = Widget()', 'lr_ref_overload(w3)' calls the lvalue version, 'lr_ref_overload(Widget())' calls the rvalue version)> 
void recieve_Widget(const Widget& x) { cout << "recieve_Widget(const &)\n"; }
void recieve_Widget(Widget& x) { cout << "recieve_Widget(&)\n"; }
void recieve_Widget(Widget&& x) { cout << "recieve_Widget(&&)\n"; }


//	TODO: 2022-03-14T23:17:54AEDT effective-c++, item 24, reference collapsing, how 'Widget&&', the rvalue to which lvalue 'x' refers, (a value moved to 'forward_Widget'/'compare_forward_alternatives') is recieved by 'recieve_Widget(Widget&)' and not 'recieve_Widget(Widget&&)' when passed without forward/move [...] -> why is forwarding not 'default' behaviour? [...] -> what are overload resolution rules where both forms of 'recieve_Widget' are declared?
//	Ongoing: 2022-03-14T00:50:06AEDT <(<interpret/explain> the results of 'forward_Widget' - even when casting to an lvalue, the type doesn't change? - std::forward changes the lvalue-ness of x, but x remains either Widget& or Widget&& with-or-without forward, instead a Widget&& (but not a Widget&) can be either an lvalue or rvalue [...] -> Widget&& is an lvalue without forward, and an rvalue with forward)> -> (and is it worth adding case 3: 'move(x)' to it?) [...] -> how passing Widget&& without forward is recieved by 'recieve_Widget(Widget&)'?
//	Ongoing: 2022-03-16T00:05:00AEDT parenthesis-ing a variable '(x)' changes its <lvalue-ness/type> for the purpouses of 'decltype'(?), but not for the purpouses of whether to call f(T&) or f(T&&)
template<typename T>
void forward_Widget(T&& x) { 
	cout << "x:\t\t";
	cout << "type=(" << get_type_name<decltype(x)>() << "), ";
	cout << "is_lvalue=(" << is_lvalue(x) << "), ";
	recieve_Widget(x);
	cout << "forward<T>(x):\t";
	cout << "type=(" << get_type_name<decltype(forward<T>(x))>() << "), ";
	cout << "is_lvalue=(" << is_lvalue(forward<T>(x)) << "), ";
	recieve_Widget(forward<T>(x));
}
//	Note: that for 'Widget&& x': 
//		'x' is an lvalue, 'forward<T>(x)' is an rvalue, (but) both have the type 'Widget&&'(?)
//		(and) it is 'is_lvalue()' that determines which lvalue/rvalue-reference overload is chosen
//	(The point of the example) (and the reason for forward being necessary), <(f(x) will always prefer f(T&) to f(T&&), even if x has type rvalue reference (x has a name, it is an lvalue), and overloading on &/&& is a branch at compile time on the lvalue/rvalueness of the argument, and not of its type)>



//	Example: when overloading by value and by rvalue reference, the call 'bad_overload(move(w1))' is ambigious
//	<(does the item about not overloading forwarding references extend to lvalue/rvalue references?)>
void bad_overload(Widget x) { cout << "bad_overload(Widget)\n"; }
void bad_overload(Widget&& x) { cout << "bad_overload(Widget&&)\n"; }



//	Ongoing: 2022-03-16T00:46:21AEDT <whether/when> to declare lvalue reference const when overloading on lvalue/rvalue references(?)

//	Example: lvalue reference version is prefered over forwarding reference version for the same cases as with 'lr_ref_overload', but 'T&&' is able to accept all arguments while 'Widget&' is not.
template<typename T> void lfr_ref_overload(T& x) { cout << "lfr_ref_overload(T&)\n"; }
template<typename T> void lfr_ref_overload(T&& x) { cout << "lfr_ref_overload(T&&)\n"; }


//	Example: an rvalue reference parameter is an lvalue
void foo(Widget&& x) {
	Widget temp1 = x;			//	calls copy-ctor
	Widget temp2 = move(x);		//	calls move-ctor
	(void) temp1; (void) temp2;
}


//	Example: Derived move ctor must call Base move ctor using 'Base(move(rhs))', since 'Base(rhs)' will call the Base copy ctor
//	<>




int&& rrefvar1 = 7;			//	rrefvar1 is an lvalue that has type rvalue-reference. <(how does it differ from 'int' -> I can modify it, take its address, and use it like an 'int'?)>

int main()
{
	//	Ongoing: 2022-03-07T02:12:31AEDT <(is it <ever> meaningful to use 'forward' outside of a function template?)>
	//	TODO: 2022-03-01T20:45:48AEDT effective-c++, item 24, forwarding-vs-rvalue-references, example (using 'is_lvalue') where forward/move have different behaviour for lvalue/rvalue-references (our examples appear to work the same?) (and what of the use of forward<T> vs forward<T&> vs forward<T&&>) ... (the topic of item 25)
	//	Ongoing: 2022-03-01T20:40:24AEDT forward<T> vs forward<T&> vs forward<T&&>? (which is correct to use for each example below?)
	//	Ongoing: 2022-03-01T20:14:19AEDT show me a difference between w1/w2/w3, or between forward() and move()? 
	//	Ongoing: 2022-03-01T19:34:58AEDT should not 'forward<Widget>(w1)' be an lvalue?
	Widget w1;
	Widget& w2 = w1;
	Widget&& w3 = Widget();
	Widget&& w4 = move(w1);
	//	{{{
	//cout << "is_lvalue(w1)=(" << is_lvalue(w1) << ")\n";
	//cout << "is_lvalue(forward(w1))=(" << is_lvalue(forward<decltype(w1)>(w1)) << ")\n";
	//cout << "is_lvalue(move(w1))=(" << is_lvalue(move(w1)) << ")\n";
	//cout << "\n";
	//cout << "is_lvalue(w2)=(" << is_lvalue(w2) << ")\n";
	//	Ongoing: 2022-03-13T00:01:24AEDT meaning of T in forward<T> -> consider forward<Widget> vs forward<decltype(w2)> (see below):
	//cout << "is_lvalue(forward<decltype(w2)>(w2))=(" << is_lvalue(forward<decltype(w2)>(w2)) << ")\n";
	//	Ongoing: 2022-03-15T22:26:39AEDT Using forward<Widget>(w2) is incorrect where decltype(w2) = Widget& (consider, if w2 is a forwarding reference 'T&& = w2', then T = Widget&)
	//cout << "is_lvalue(forward<Widget>(w2))=(" << is_lvalue(forward<Widget>(w2)) << ")\n";
	//cout << "is_lvalue(move(w2))=(" << is_lvalue(move(w2)) << ")\n";
	//cout << "\n";
	//cout << "is_lvalue(w3)=(" << is_lvalue(w3) << ")\n";
	//cout << "is_lvalue(forward(w3))=(" << is_lvalue(forward<decltype(w3)>(w3)) << ")\n";
	//cout << "is_lvalue(move(w3))=(" << is_lvalue(move(w3)) << ")\n";
	//cout << "\n";
	//cout << "is_lvalue(w4)=(" << is_lvalue(w4) << ")\n";
	//cout << "is_lvalue(forward(w4))=(" << is_lvalue(forward<decltype(w4)>(w4)) << ")\n";
	//cout << "is_lvalue(move(w4))=(" << is_lvalue(move(w4)) << ")\n";
	//cout << "\n";
	//	}}}

	//	Example: decltype(x) <(is this type deduction for our purpouses?)>
	//	If 'x' is the name of a variable (an id-expression), the resulting type is the type of that variable
	//	Otherwise, 
	//		if 'x' evalutes to an lvalue T, the resulting type is 'T&'
	//		if 'x' evalutes to an rvalue T, the resulting type is 'T'
	decltype(w1) r1;					//	T = Widget
	decltype((w1)) r2 = w1;				//	T = Widget& (since decltype((w1)) = T&)
	decltype((w1))&& r3 = w1;			//	T = Widget& (since decltype((w1)) = T&)
	decltype(w1)&& r4 = move(w1);		//	T = Widget&&


	forward_Widget(w1);
	forward_Widget(move(w1));
	cout << "\n";

	recieve_Widget(w1);
	recieve_Widget(move(w1));
	recieve_Widget(w3);
	recieve_Widget(move(w3));
	recieve_Widget(Widget());
	cout << "\n";
	lfr_ref_overload(w1);
	lfr_ref_overload(move(w1));
	lfr_ref_overload(w3);
	lfr_ref_overload(move(w3));
	lfr_ref_overload(Widget());
	cout << "\n";

	(void) r1; (void) r2; (void) r3; (void) r4; (void) w2; (void) w3; (void) w4;
	return 0;
}

//	Summary:
//		If a function template parameter has type 'T&&' for a deduced type T, or if an object is declared using 'auto&&', the variable is a forwarding reference. If the declaration is const or volatile, the variable is an rvalue reference.
//		Forwarding references correspond to lvalue/rvalue-references, according to lvalue/rvalue-ness of the variable they are initalized with.

