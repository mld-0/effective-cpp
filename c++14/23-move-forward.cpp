//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <random>
using namespace std;
//	{{{2
//	is_lvalue(T&&), get_vector_randoms(int,int,int), get_type_name<T>()
//	{{{
//	'is_lvalue()', determine is argument is lvalue/rvalue 
template<typename T>
constexpr bool is_lvalue(T&& x) {
//	{{{
	return std::is_lvalue_reference<T>{};
}
//	}}}
//	'get_vector_randoms()', 
vector<int> get_vector_randoms(int size, int min=0, int max=9) {
//	{{{
	//	values generated are in range [min, max] 
    std::uniform_int_distribution<int> distribution(min, max);
    std::default_random_engine generator;
	vector<int> result(size);
	auto gen = [&distribution,&generator]() { return distribution(generator); };
	std::generate(result.begin(), result.end(), gen);
	return result;
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
class Widget;

//	Ongoing: 2022-03-02T02:24:26AEDT document should be named 'move-vs-forward' (or 'move-and-forward') (<why/how> is git (still) so bad with renaming files?)

//	TODO: 2022-03-07T05:41:32AEDT effective-c++, item 23, move-forward, lambda vs function template type deduction rules, lambda with parameters 'auto&& x' vs template function 'T&& x' (difference in type deduction rules) <(try using decltype(auto)?)> (deduced type of x, and reconsiling that (for 'l_steal_.*', 'T = typename std::remove_reference<decltype(x)>::type') <- (still looking for a simpler solution / way to reconcile) -> (since 'decltype(auto)' is invalid for a lambda parameter) -> (is 'decltype(auto)' useable for any<one's> arguments? 'decltype(auto)' only for return type and variable declarations?) [...] -> <something-something only difference between template/auto type deduction?> (from item 02): "when the initalizer for an auto variable is enclosed in braces {}, the deduced type is initalizer_list, this is the <only?> difference between auto type deduction and template type deduction" <(revise (item 02) statement, clarification of the '<only?> after addressing topic?  2022-03-07T05:49:32AEDT

//	TODO: 2022-03-07T06:10:37AEDT effective-c++, item 23, move-forward, what is the meaning of the type T used in 'forward<T>(x)', (and from there, is 'forward<decltype(x)>(x)' always valid?)

//	TODO: 2022-03-03T02:53:00AEDT effective-c++, item 23, move-forward, definition/example for 'std::move_if_noexcept'

//	TODO: 2022-03-02T02:24:05AEDT effective-c++, item 23, move-forward, review implicit move assignment/ctor rules (having copied the ctor rules to be cleaned up, along with a statement of presumption that implicit move assignment works the same way?)

//	TODO: 2022-03-06T20:49:18AEDT effective-c++, item 23, move-forward, is calling 'forward' on a non-forwarding-reference ever meaningful/correct (considering our "show me the difference between forward/move dilemma previously"?)

//	On l/r-value-ness:
//	{{{
//	Generally, an lvalue is a variable whose address can be taken (whereas an rvalue is one whose address cannot be taken).
//	l/r-valueness is independent of type.
//class Widget_i {
//public:
//	Widget_i(Widget_i&& rhs);		//	rhs is an lvalue, even though it has an rvalue reference type
//};
//	Taking the address of 'rhs' in the move-constructor would be valid, hence it is an lvalue (all parameters are lvalues, but they arguments with which they are initalized may be lvalues or rvalues). 
//	Perfect forwarding is passing an argument while preserving the lvalue/rvalue-ness of the argument (see item 30).
//	Copies of rvalues are generally moved-constructed, while copies of lvalues are usually copy-constructed
//	Besides l/r-values, we have:
//		xvalue: 'eXpiring' value, refers to an object near the end of its lifetime (example: the result of calling a function whose return type is an rvalue reference is an xvalue)
//		glvalue: 'generalized' lvalue, either an lvalue or an xvalue.
//		prvalue: 'pure' rvalue, is an rvalue that is not an xvalue (example: the result of calling a function whose return type is not a reference)
//	}}}

//	rvalue references are the language feature that makes both move-semantics and perfect-forwarding possible. 
//	'T&&' has two meanings. It can mean rvalue reference, or it can mean forwarding reference. A forwarding reference is an rvalue reference to a cv-unqualified template parameter. A forwarding reference can behave as either an lvalue reference 'T&' or an rvalue reference 'T&&'. If 'T&&' is used in a context without type deduction, it is an rvalue reference not a forwarding reference (see item 24).


//	Move Semantics: makes it possible to replace expensive copies with cheaper moves. The move-ctor and move-assignment member functions control the semantics of moving.  <(<Default/implicit> class support for move?)> <(Move only types (are those with deleted copy ctor/assignment?))>
//	Perfect Forwarding: makes it possible to write function templates that take arbitrary arguments and forward them to a third function as they were received.

//	Default/Implicit move assignment/constructor behaviour (see item 17):
//	{{{
//		'= default' move ctor/assign performs a move on each class member variable
//		The implicit move ctor</assign?> of a class is created if: <(1: the class does not have any other special member functions declared <anything, like any ctor, any copy, ect?>, and 2: the move ctor can be sensibly implemented by moving all its members and bases (what is 'members and bases'), (how is the compiler assessing this? is it a question of is move supported for all member variables?))>
//	Rule (cppreference):
//	{{{
//	LINK: https://en.cppreference.com/w/cpp/language/move_constructor
//	If no user-defined move constructors are provided for a class type (struct, class, or union), and all of the following is true:
//	there are no user-declared copy constructors;
//	there are no user-declared copy assignment operators;
//	there are no user-declared move assignment operators;
//	there is no user-declared destructor.
//	then the compiler will declare a move constructor as a non-explicit inline public member of its class with the signature T::T(T&&).
//	A class can have multiple move constructors, e.g. both T::T(const T&&) and T::T(T&&). If some user-defined move constructors are present, the user may still force the generation of the implicitly declared move constructor with the keyword default.
//	The implicitly-declared (or defaulted on its first declaration) move constructor has an exception specification as described in dynamic exception specification (until C++17)noexcept specification (since C++17)
//	}}}
//	Ongoing: 2022-03-02T02:22:12AEDT can one presume the implicit move-assignment operator follows same/similar rules?
//	}}}


//	A function parameter is always an lvalue, even when it has the type rvalue reference:
void f(Widget&& w_arg); 


//	Example: std::move() approximation, casts 'param' to an rvalue (for reference collapsing see item 28)
//	The result of applying move to an object is that it is cast to an rvalue. This indicates its contents is eligible for stealing to the callee. 
//	The value of a variable <(C++ parlance object)> <(excluding primatives?)> which has been moved may have been erased. Do not use the value of a variable after it has been moved.
template<typename T>
decltype(auto) my_move(T&& param) {	
	using RT = typename remove_reference<T>::type&&;
	return static_cast<RT>(param);
}
//	Under some circumstances, a move may be silently (without compiler warning) be replaced with a copy:
//		move is not implemented for the type in question
//		caller variable passed is const
//		<(what about 'const T&&' a const rvalue/forwarding reference?)>
//		<>



//	Ongoings:
//	{{{
//	Ongoing: 2022-03-08T03:02:00AEDT can there be a const forwarding reference
//	Ongoing: 2022-03-07T02:08:33AEDT declaration 'std::remove_reference<T>::type&& t' is not type deduction for purpouses of forwarding reference(s)?
//	Ongoing: 2022-03-07T02:07:21AEDT purpose of static_assert in rvalue version of 'my_forward()'?
//	}}}
//	Example: std::forward<T>() approximation, conditional version of 'move', it performs a move (cast to rvalue-ref) only if its argument was initialized with an rvalue (recall that parameters are lvalues)
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}


//	Example: 'move's on const variables are silently transformed into copys, (and no compiler warning is given).
class Annotation {
public:
	explicit Annotation(const string newValue)
		: value(move(newValue))					//	attempt to move const variable produces copy
	{}
	string value;
};


//	Ongoings:
//	{{{
//	Ongoing: 2022-03-07T03:41:42AEDT (this does not answer the question, whether the only use of forward is in (if not function templates, then functions with deduced parameter types))
//	Ongoing: 2022-03-07T03:44:13AEDT (difference(s) between template functions, and functions with deduced parameters)
//	Ongoing: 2022-02-26T02:02:20AEDT auto-initialization from move/forward?
//	Ongoing: 2022-02-26T02:04:24AEDT implementing 'move(var_str)' as 'forward<string>(var_str)'? (see example Widget move ctor)
//	Ongoing: 2022-03-06T20:00:35AEDT The result of calling move/forward on a non-reference variable is a reference(?)
//	}}}


//	Example: value stealing function template, (with and without use of 'forward')
template<typename T>
void f_steal_forward(T&& x) {
	//T temp{ std::forward<T>(x) };
	T temp( std::forward<T>(x) );
	(void) temp;
}
//	Ongoing: 2022-03-08T03:07:17AEDT 'noforward' functions being pointless (once our exploration of the behaviour of functions is complete)
template<typename T>
void f_steal_noforward(T&& x) {
	T temp( x );
	(void) temp;
}

//	Ongoing: 2022-03-07T04:07:40AEDT comparing (forwarding references for) template-functions vs auto-parameter-lambdas)
template<typename T>
void f_islvalue_noforward(T&& x) {
	//cout << "l_islvalue_noforward=(" << is_lvalue(x) << ")\n";
	cout << "l_islvalue_noforward=(" << is_lvalue(x) << ")\n";
}
template<typename T>
void f_islvalue_forward(T&& x) {
	//cout << "l_islvalue_forward=(" << is_lvalue(std::forward<decltype(x)>(x)) << ")\n";
	cout << "l_islvalue_forward=(" << is_lvalue(std::forward<T>(x)) << ")\n";
}

//	Ongoings:
//	{{{
//	Ongoing: 2022-03-07T06:00:14AEDT we already have a the TO-DO for <the-case-of> 'f(T, T&&)', (after which), determine (if there is a) non-overloaded 1-function solution to forward lvalues/rvalues to vector<T>::emplace_back [...] (might (item 26) have something to offer?)
//	Ongoing: 2022-03-07T05:56:19AEDT 'auto' not allowed in function <prototype?> <that is as used here> (yet StackOverflow example (see below) gives us a <member function?> with auto in it?
//	Ongoing: 2022-03-07T05:53:26AEDT (I have hear the claim) (StackOverflow) that 'decltype(value)(value)' is equivalent to 'std::forward<decltype<value)>(value)'? (see below)
//void push_back(vector<T> vec, auto&& value) {			//	<(Provided <confidently> as StackOverflow example (auto in function declarations</definitions?> not allowed?))>
//	}}}

//	Example: 'my_push_back()'
template<typename T>
void my_push_back(vector<T> vec, T&& value) {
	//	LINK: https://stackoverflow.com/questions/66726363/perfect-forwarding-a-value-of-type-auto
	vec.emplace_back(std::forward<T>(value));
}




int main()
{
	//	Ongoings: 
	//	{{{
	//	Ongoing: 2022-02-26T23:13:49AEDT need examples of use of move/forward (since I find myself unable to call forward()? -> because forward must be used as 'forward<T>(x)'?)
	//	Ongoing: 2022-03-07T03:50:42AEDT lambda (with auto deduced parameter) did not report that function called within 'move_steal_value' did not exist until it was called (instantiated) -> (which leads to the question) how might that call to a non-existant function be valid for some passed in type?)
	//	Ongoing: 2022-03-07T03:57:10AEDT forwarding from lambda steal_forward -> f_steal_forward -> temp ctor, (which) doesn't clear 'v1'(?) [...] -> same for lambda -> temp ctor (the hangup is we do not appear to be passing an rvalue? i.e: need to call 'l_steal_forward(forward<>(v1))'(?)) -> (the point being, are there different rules for forwarding references for auto-lambda-parameters as template functions?) (... would the results be the same were we using 'decltype(x)' (in place of 'T') in the template function?) [...] -> (conclusion? the problem is our lack of 'move' on the caller side -> 'x' is instantiated from an lvalue, and therefore forward (correctly) passes it as an lvalue, passing an lvalue to a function recieving a forwarding reference is <give-me-an-exact-type? that-is, reference-or-no?> recieved as <(a reference? satisfy me vis-a-vis <the-three> rules of template type deduction)> )
	//	Ongoing: 2022-03-07T06:04:59AEDT (given our problem here needing to use more than 'decltype(x)' to deduce T equivalent to function template 'T&& x' for lambda 'auto&& x' -> is the confident example suggesting 'std::forward<decltype(x)>(x)' always correct? (the error was with the type of 'temp' given the argument for the ctor (remind me: what was it?), using 'T temp( forward<T>(x) )' appears to <also> be correct.
	//	Ongoing: 2022-03-07T05:27:17AEDT problem with our usage of decltype(x) for 'l_steal_noforward' -> need to <de-reference> 'x' first? (since simply (cheating and using) 'vector<int>' works?) -> (what were we getting from 'decltype(x)' ... where x is 'vector<int>&'?) [...] -> (problem with 'l_steal_forward' is use of 'forward<decltype(x)>' (ditto: need to remove reference-ness?))
	//	}}}

	//	Example: using 'forward' with 'auto' types (<in/for> lambdas)
	//	<(for a lambda with auto parameter 'auto&& x', decltype(x) does not equal T from the equivalent 'T&& x' template function? -> needed to )>
	auto l_steal_noforward = [] (auto&& x) { 
		using T = typename std::remove_reference<decltype(x)>::type;
		T temp( x ); 
		(void) temp; 
	};
	auto l_steal_forward = [] (auto&& x) { 
		using T = typename std::remove_reference<decltype(x)>::type;
//	Ongoing: 2022-03-07T05:39:25AEDT (for both f_steal_.* and l_steal_.*) difference between 'T temp{ forward<T>(x) }' and 'T temp( forward<T>(x) )'?
		T temp( std::forward<T>(x) );
		//T temp( std::forward<decltype(x)>(x) );		//	<(seemingly correct (see above))>
		(void) temp; 
	};


	auto l_islvalue_noforward = [] (auto&& x) { cout << "l_islvalue_noforward=(" << is_lvalue(x) << ")\n"; };
	auto l_islvalue_forward = [] (auto&& x) { cout << "l_islvalue_forward=(" << is_lvalue(std::forward<decltype(x)>(x)) << ")\n"; };

	//	Ongoing: 2022-03-07T06:16:37AEDT (clean up calls below later), (currently investigating move vs no-move, and type deduction (WRT forwarding references) for lambda auto parameters vs function template types), (and the ongoing question: is it ever valid to call forward outside a function with a forwarding reference as parameter?)
	vector<int> v1 = get_vector_randoms(20);
	cout << "v1.size()=(" << v1.size() << ")\n";
	l_steal_noforward(v1);
	cout << "l_steal_noforward, v1.size()=(" << v1.size() << ")\n";
	l_steal_forward(v1);
	cout << "l_steal_forward, v1.size()=(" << v1.size() << ")\n";
	f_steal_noforward(v1);
	cout << "f_steal_noforward, v1.size()=(" << v1.size() << ")\n";
	f_steal_forward(v1);
	cout << "f_steal_forward, v1.size()=(" << v1.size() << ")\n";
	cout << "\n";
	vector<int> v3_fl = get_vector_randoms(20);
	l_steal_noforward(move(v3_fl));						//	invalid, <>
	cout << "l_steal_noforward(move), v3_fl.size()=(" << v3_fl.size() << ")\n";
	l_steal_forward(move(v3_fl));
	cout << "l_steal_forward(move), v3_fl.size()=(" << v3_fl.size() << ")\n";
	vector<int> v3_ff = get_vector_randoms(20);
	f_steal_noforward(move(v3_ff));
	cout << "f_steal_noforward(move): v3_ff=(" << v3_ff.size() << ")\n";
	f_steal_forward(move(v3_ff));
	cout << "f_steal_forward(move): v3_ff=(" << v3_ff.size() << ")\n";
	cout << "\n";
	vector<int> v2 = get_vector_randoms(20);
	cout << "l_islvalue_noforward, ";
	l_islvalue_noforward(v2);
	cout << "l_islvalue_forward, ";
	l_islvalue_forward(v2);
	cout << "f_islvalue_noforward, ";
	f_islvalue_noforward(v2);
	cout << "f_islvalue_forward, ";
	f_islvalue_forward(v2);
	cout << "\n";
	vector<int> v3_m = get_vector_randoms(20);
	cout << "l_islvalue_noforward(move), ";
	l_islvalue_noforward(move(v3_m));
	cout << "l_islvalue_forward(move), ";
	l_islvalue_forward(move(v3_m));
	cout << "f_islvalue_noforward(move), ";
	f_islvalue_noforward(move(v3_m));
	cout << "f_islvalue_forward(move), ";
	f_islvalue_forward(move(v3_m));
	cout << "\n";


	//	Usage: my_push_back(vector<T>, T&&)
	vector<int> eb_v1;
	int eb_i1 = 53;
	my_push_back(eb_v1, 27);
	//my_push_back(eb_v1, eb_i1);			//	error, conflicting types
	my_push_back(eb_v1, move(eb_i1));


	//	Ongoing: 2022-03-07T06:19:12AEDT does move-ing a primative ever clear it? (does it make sense <at-all/ever> to move primatives?)
	(void) eb_i1;
	return 0;
}

//	Summary:
//		'std::move()' performs an unconditional cast to an rvalue
//		'std::forward()' performs a cast to rvalue <only> if <(the value given as argument is bound to an rvalue?)> <(the value given as argument was initalized as an rvalue?)>

