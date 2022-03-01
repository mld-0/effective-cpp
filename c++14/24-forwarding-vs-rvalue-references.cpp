#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;
class Widget {};

//	Ongoing: 2022-02-26T02:06:45AEDT should this not be covered before move/forward (item 23)
//	<(Here we will make a simplifications about '&&'?)>

//	'T&&' has two meanings. It can mean rvalue reference, or it can mean forwarding reference.
//	A forwarding reference is an rvalue reference to a cv-unqualified template parameter. A forwarding reference can behave as either an lvalue reference 'T&' or an rvalue reference 'T&&'. 
//	<(bind to const, bind to volatile)>.
//	If 'T&&' is used in a context without type deduction, it is an rvalue reference not a forwarding reference.

//	Forwarding references, (like all references) must be initialized.
//		If a forwarding reference is initalized with an lvalue, it acts as an lvalue reference
//		If a forwarding reference is initalized with an rvalue, it acts as an rvalue reference


//	Ongoing: 2022-03-02T02:12:02AEDT <(I <dispute/question/fail-to-see-how> how 'is_lvalue(forward(x))' gives the same result for 'int&&' and 'int' arguments of 'x')>
//	Example: determine is argument is lvalue/rvalue 
template<typename T>
constexpr bool is_lvalue(T&& x) {
	return std::is_lvalue_reference<T>{};
}

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

//	Reference Collapsing: (see item 28)
//		A& &		A&
//		A&& &		A&
//		A& &&		A&
//		A&& &&		A&&


int&& rrefvar1 = 7;			//	rrefvar1 is an lvalue that has type rvalue-reference. <(how does it differ from 'int' -> I can modify it, take its address, and use it like an 'int'?)>

int main()
{
	//	TODO: 2022-03-01T20:45:48AEDT effective-c++, item 24, forwarding-vs-rvalue-references, example (using 'is_lvalue') where forward/move have different behaviour for lvalue/rvalue-references (our examples appear to work the same?) (and what of the use of forward<T> vs forward<T&> vs forward<T&&>) ... (the topic of item 25)
	//	Ongoing: 2022-03-01T20:40:24AEDT forward<T> vs forward<T&> vs forward<T&&>? (which is correct to use for each example below?)
	//	Ongoing: 2022-03-01T20:14:19AEDT show me a difference between w1/w2/w3, or between forward() and move()? 
	//	Ongoing: 2022-03-01T19:34:58AEDT should not 'forward<Widget>(w1)' be an lvalue?
	Widget w1;
	cout << "is_lvalue(w1)=(" << is_lvalue(w1) << ")\n";
	cout << "is_lvalue(forward(w1))=(" << is_lvalue(forward<Widget>(w1)) << ")\n";
	cout << "is_lvalue(move(w1))=(" << is_lvalue(move(w1)) << ")\n";
	cout << "\n";
	Widget& w2 = w1;
	cout << "is_lvalue(w2)=(" << is_lvalue(w2) << ")\n";
	cout << "is_lvalue(forward(w2))=(" << is_lvalue(forward<Widget>(w2)) << ")\n";
	cout << "is_lvalue(move(w2))=(" << is_lvalue(move(w2)) << ")\n";
	cout << "\n";
	Widget&& w3 = Widget();
	cout << "is_lvalue(w3)=(" << is_lvalue(w3) << ")\n";
	cout << "is_lvalue(forward(w3))=(" << is_lvalue(forward<Widget>(w3)) << ")\n";
	cout << "is_lvalue(move(w3))=(" << is_lvalue(move(w3)) << ")\n";
	cout << "\n";

	//	Example: decltype(x) <(is this type deduction for our purpouses?)>
	//	If 'x' is the name of a variable (an id-expression), the resulting type is the type of that variable
	//	Otherwise, 
	//		if 'x' evalutes to an lvalue T, the resulting type is 'T&'
	//		if 'x' evalutes to an rvalue T, the resulting type is 'T'
	decltype(w1) r1;					//	T = Widget
	decltype((w1)) r2 = w1;				//	T = Widget& (since decltype((w1)) = T&)
	decltype(w1)&& r3 = move(w1);		//	T = Widget&&
	decltype((w1))&& r4 = w1;			//	T = Widget& (since decltype((w1)) = T&)


	(void) r1; (void) r2; (void) r3; (void) r4;
	return 0;
}

//	Summary:
//		If a function template parameter has type 'T&&' for a deduced type T, or if an object is declared using 'auto&&', the variable is a forwarding reference. If the declaration is const or volatile, the variable is an rvalue reference.
//		Forwarding references correspond to lvalue/rvalue-references, according to lvalue/rvalue-ness of the variable they are initalized with.

