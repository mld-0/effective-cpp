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
class Widget {};

//	Ongoing: 2022-03-16T00:38:59AEDT move ctor/assignment functions must 1) not throw exceptions, and 2) be designated noexcept, for moved to be used in some situations (i.e: vector resizing) <(this belongs with the item outlining move assign/ctor functions)>
//	Ongoing: 2022-03-11T00:27:29AEDT meaing of 'decltype(x)&&'?
//	Ongoing: 2022-03-02T02:24:26AEDT document should be named 'move-vs-forward' (or 'move-and-forward') (<why/how> is git (still) so bad with renaming files?)
//	TODO: 2022-03-07T05:41:32AEDT effective-c++, item 23, move-forward, lambda vs function template type deduction rules, lambda with parameters 'auto&& x' vs template function 'T&& x' (difference in type deduction rules) <(try using decltype(auto)?)> (deduced type of x, and reconsiling that (for 'l_steal_.*', 'T = typename std::remove_reference<decltype(x)>::type') <- (still looking for a simpler solution / way to reconcile) -> (since 'decltype(auto)' is invalid for a lambda parameter) -> (is 'decltype(auto)' useable for any<one's> arguments? 'decltype(auto)' only for return type and variable declarations?) [...] -> <something-something only difference between template/auto type deduction?> (from item 02): "when the initalizer for an auto variable is enclosed in braces {}, the deduced type is initalizer_list, this is the <only?> difference between auto type deduction and template type deduction" <(revise (item 02) statement, clarification of the '<only?> after addressing topic?  2022-03-07T05:49:32AEDT
//	TODO: 2022-03-07T06:10:37AEDT effective-c++, item 23, move-forward, what is the meaning of the type T used in 'forward<T>(x)', (and from there, is 'forward<decltype(x)>(x)' always valid?)
//	TODO: 2022-03-03T02:53:00AEDT effective-c++, item 23, move-forward, definition/example for 'std::move_if_noexcept'
//	TODO: 2022-03-02T02:24:05AEDT effective-c++, item 23, move-forward, review implicit move assignment/ctor rules (having copied the ctor rules to be cleaned up, along with a statement of presumption that implicit move assignment works the same way?)
//	TODO: 2022-03-06T20:49:18AEDT effective-c++, item 23, move-forward, is calling 'forward' on a non-forwarding-reference ever meaningful/correct (considering our "show me the difference between forward/move dilemma previously"?)
//	Ongoing: 2022-03-18T01:44:36AEDT come the time to combine items 23-30, include 'extra/lvalues-rvalues-references'(?) - since with the complete item (presumedly) there will be no need for it (is there anything not surpurfulous there anyway?)


//	On l/r-value-ness:
//	Generally, an lvalue is a variable whose address can be taken (whereas an rvalue is one whose address cannot be taken). l/r-valueness is independent of type.
//	Perfect forwarding is passing an argument while preserving the lvalue/rvalue-ness of the argument (see item 30).
//	Copies of rvalues are generally moved-constructed, while copies of lvalues are usually copy-constructed
//	Besides l/r-values, we have:
//		xvalue: 'eXpiring' value, refers to an object near the end of its lifetime (example: the result of calling a function whose return type is an rvalue reference is an xvalue)
//		glvalue: 'generalized' lvalue, either an lvalue or an xvalue.
//		prvalue: 'pure' rvalue, is an rvalue that is not an xvalue (example: the result of calling a function whose return type is not a reference)

//	rvalue references are the language feature that makes both move-semantics and perfect-forwarding possible. 
//	'T&&' has two meanings. It can mean rvalue reference, or it can mean forwarding reference. A forwarding reference is an rvalue reference to a cv-unqualified template parameter. A forwarding reference can behave as either an lvalue reference 'T&' or an rvalue reference 'T&&'. If 'T&&' is used in a context without type deduction, it is an rvalue reference not a forwarding reference (see item 24).


//	Move Semantics: makes it possible to replace expensive copies with cheaper moves. The move-ctor and move-assignment member functions control the semantics of moving.  <(<Default/implicit> class support for move?)> <(Move only types (are those with deleted copy ctor/assignment?))>
//	Perfect Forwarding: makes it possible to write function templates that take arbitrary arguments and forward them to a third function as they were received.
//	Some kinds <types?> of arguments cannot be perfect-forwarded (see item 30)


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
//	Ongoing: 2022-03-16T00:17:28AEDT <(A move assignment/ctor should take care of those parts of the dtor that have side effects (for the value being stolen from))> <(this detail belongs with the item outlining move ctor/assign functions)>


//	A function parameter is always an lvalue (as it has a name, and taking its address is valid), even when it has the type rvalue reference:
void f(Widget&& w_arg); 


//	'std::move()' approximation:
//	Casts the forwarding reference 'param' to an rvalue. When passing a variable this indicates to the callee that its contents is eligible for stealing.
//	The implementation of move depends on the type in question. Many types are left with an undefined value after being moved. Do not use the value of a variable after it has been moved.
template<typename T>
decltype(auto) my_move(T&& param) {	
	using RT = typename remove_reference<T>::type&&;
	return static_cast<RT>(param);
}
//	Under some circumstances, a move may be silently (without compiler warning) be replaced with a copy:
//		fundamental types do not define moves
//		move is not implemented for the type in question
//		caller variable passed is const
//		<(what about 'const T&&' a const rvalue/forwarding reference?)>
//		<>


//	Ongoings:
//	{{{
//	Ongoing: 2022-03-08T03:02:00AEDT can there be a const forwarding reference
//	Ongoing: 2022-03-07T02:08:33AEDT declaration 'std::remove_reference<T>::type&& t' is not type deduction for purpouses of forwarding reference(s)?
//	Ongoing: 2022-03-07T02:07:21AEDT purpose of static_assert in rvalue version of 'my_forward()'?
//	LINK: https://isocpp.org/blog/2018/02/quick-q-whats-the-difference-between-stdmove-and-stdforward
//	Ongoing: 2022-03-14T00:03:42AEDT (how might even one trigger (each of the) 'my_forward' static_assert error messages?) (and is the 'is_rvalue_reference' on correct?)
//	}}}

//	'std::forward<T>()' approximation:
//	Conditional version of 'move', it performs a move (cast to rvalue reference) only if its argument was initialized with an rvalue (recall that parameters are lvalues). 
//	<(std::forward has a single use case: to cast a templated function parameter (inside the function) to the value category (lvalue or rvalue) the caller used to pass it. This allows rvalue arguments to be passed on as rvalues, and lvalues to be passed on as lvalues, a scheme called perfect forwarding)>
//	When passed an lvalue, forward<T>(T&) is called, T is deduced as T&, and the result is the cast T& && = T& 
//	When passed an rvalue, forward<T>(T&&) is called, T is deduced as T, and the result is the cast T&&
//	Ongoing: 2022-03-14T00:00:23AEDT <C++14> 'remove_reference_t<T>' gives a neater implementation than 'typename std::remove_reference<T>::type'(?)
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type& t) noexcept {
	//	Ongoing: 2022-03-13T19:12:19AEDT any chance for a static_assert w/ error message for (some kind of) wrong usage case for 'my_forward(&)'? [...] -> <(is this (see below) 'static_assert' error message correct for purpouse?)> 
	static_assert(!std::is_rvalue_reference<T>::value, "Can not forward an lvalue as an rvalue.");
    return static_cast<T&&>(t);
}
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}
//	TODO: 2022-03-13T22:26:02AEDT (Do I remember right) <('decltype(x)(x)' is equivalent to 'forward<decltype(x)>(x)'?)> (and <even> if it is, the clarity of the latter is worth the extra characters?)


//	Not a useful example(?) <(where else be a warning about const becoming a silent copy (with no compiler warning))>
//	{{{
//	Example: 'move's on const variables are silently transformed into copys, (and no compiler warning is given).
class Annotation {
public:
	explicit Annotation(const string newValue)
		: value(move(newValue))					//	attempt to move const variable produces copy
	{}
	string value;
};
//	}}}

//	Ongoings:
//	{{{
//	Ongoing: 2022-03-07T03:41:42AEDT (this does not answer the question, whether the only use of forward is in (if not function templates, then functions with deduced parameter types))
//	Ongoing: 2022-03-07T03:44:13AEDT (difference(s) between template functions, and functions with deduced parameters)
//	Ongoing: 2022-02-26T02:02:20AEDT auto-initialization from move/forward?
//	Ongoing: 2022-02-26T02:04:24AEDT implementing 'move(var_str)' as 'forward<string>(var_str)'? (see example Widget move ctor)
//	Ongoing: 2022-03-06T20:00:35AEDT The result of calling move/forward on a non-reference variable is a reference(?)
//	Ongoing: 2022-03-07T05:39:25AEDT (for both f_steal_.* and l_steal_.*) difference between 'T temp{ forward<T>(x) }' and 'T temp( forward<T>(x) )'?
//	Ongoing: 2022-03-14T00:06:16AEDT make a note that the deducing T = auto problem is (tangentially) <related/relevant> to f/l_steal_forward, and clear out all this <deliberation> to said example, finally leaving a note to incorporate <findings/solution> for what type should I make my steal-ey vector and how can I get what T would have been for whatever reason it might be needed, does making these message unnecessarily long and ramble-ey detract from their <purpose/usefulness> <no time for spellchecking> <WPM was 22.5> 2022-03-14T00:09:54AEDT
//	}}}


//	Ongoing: 2022-03-10T01:29:06AEDT the moral of the story is that ... applying forward<T> with the wrong type gives the wrong result (in this case -> move-ing something in an lvalue reference) -> leading back to the question of the meaning of the type forward is instantiated with (vs that passed to it) [...] the solution (here) though, is deducing the correct type for 'TD'(?) -> our lambda function has two decltype(x) -> which is the issue? -> the first one is whatever-works, for the second one, 'decltype(x)' seems to work fine (even though decltype(x) != T), but TD not matching T is the issue. (Also revealed here: A callee can move something that was never moved to it -> a lesson <prehaps> in the danger of non-cost references?)
//	Ongoing: 2022-03-10T02:15:39AEDT calling 'T&&' with an lvalue yields T&, with an rvalue yields T&& -> (our error has been using TD which doesn't match T (and also not understanding deduced type rules)) [...] -> (to get TD, remove reference-ness, only if it is an rvalue reference) (doesn't that leave us trying to declare a 'vector<int>&'?)
//	Ongoing: 2022-03-10T03:44:39AEDT if it is 'canonical' to use 'forward<decltype(x)>(x)' -> does that imply it is acceptable for the <> type to be an rvalue-ref? TO-DO is the meaning of this type. 


//	Example: value stealing, function template vs auto parameter lambda
template<typename T>
void f_steal_forward(T&& x) {
	//using TD = typename std::remove_reference<decltype(x)>::type;
	//	TODO: 2022-03-10T03:41:22AEDT effective-c++, item 23, move-forward, latest problem comes down to: given 'f(auto&& x)', determine what T would be if we were using 'f(T&& x)' (specifically here - make T == TD)
	using TD = decltype(x);
	cout << "T=(" << get_type_name<T>() << ")\n";
	cout << "TD=(" << get_type_name<TD>() << ")\n";
	//T temp{ std::forward<T>(x) };
	T temp( std::forward<T>(x) );
	cout << "get_type_name(temp)=(" << get_type_name<decltype(temp)>() << ")\n";
	(void) temp;
}
//	<(for a lambda with auto parameter 'auto&& x', decltype(x) does not equal T from the equivalent 'T&& x' template function? -> needed to )>
auto l_steal_forward = [] (auto&& x) { 
	//	incorrect: produces 'vector<int>&&' when we need 'vector<int>'
	//using TD = decltype(x);
	//	Ongoing: 2022-03-10T04:02:25AEDT both 'TD's are correct for the purpouses of a 'stealing' function, but neither is correct in the 'T == TD' sense 
	//using TD = typename std::remove_reference<decltype(x)>::type;
	using TD = typename std::decay<decltype(x)>::type;
	//	{{{
	//using TD = decltype(std::forward<decltype(x)>(x));
	//using TD = decltype(x)&&;
	//using TD = decltype(x)&&;
	//using TD = decltype(forward<decltype(x)>(x));
	//using TD = decltype(x);
	//TD temp( std::forward<TD>(x) );
	//T temp( std::forward<decltype(x)>(x) );		//	<(seemingly correct (see above))>
	//	}}}
	//	Usage of 'forward<decltype(x)>()' is correct
	//TD temp( std::forward<decltype(x)>(x) );
	//	Ongoing: 2022-03-13T18:07:02AEDT <this problem> comes back to trying to deduced T in order to declare 'temp' -> (but all this time we could have just used) 'auto'?
	auto temp = std::forward<decltype(x)>(x);
	cout << "get_type_name(x)=(" << get_type_name<decltype(x)>() << ")\n";
	cout << "get_type_name(temp)=(" << get_type_name<decltype(temp)>() << ")\n";
	(void) temp; 
};


//	Ongoing: 2022-03-07T04:07:40AEDT comparing (forwarding references for) template-functions vs auto-parameter-lambdas)
template<typename T>
void f_islvalue_forward(T&& x) {
	//cout << "l_islvalue_forward=(" << is_lvalue(std::forward<decltype(x)>(x)) << ")\n";
	//cout << get_type_name<decltype(x)>() << "\n";
	cout << "f_islvalue=(" << is_lvalue(x) << "), ";
	cout << "f_islvalue_forward=(" << is_lvalue(std::forward<T>(x)) << ")\n";
}
auto l_islvalue_forward = [] (auto&& x) { 
	//cout << get_type_name<decltype(x)>() << "\n";
	cout << "l_islvalue=(" << is_lvalue(x) << "), "; 
	cout << "l_islvalue_forward=(" << is_lvalue(std::forward<decltype(x)>(x)) << ")\n"; 
};


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

	//	Ongoing: 2022-03-07T06:16:37AEDT (clean up calls below later), (currently investigating move vs no-move, and type deduction (WRT forwarding references) for lambda auto parameters vs function template types), (and the ongoing question: is it ever valid to call forward outside a function with a forwarding reference as parameter?)

	vector<int> v1 = get_vector_randoms(20);
	cout << "l_steal_forward:\n";
	l_steal_forward(v1);
	cout << "v1.size()=(" << v1.size() << ")\n";
	cout << "\n";

	v1 = get_vector_randoms(20);
	cout << "f_steal_forward:\n";
	f_steal_forward(v1);
	cout << "v1.size()=(" << v1.size() << ")\n";
	cout << "\n";

	vector<int> v3_fl = get_vector_randoms(20);
	cout << "l_steal_forward:\n";
	l_steal_forward(move(v3_fl));
	cout << "l_steal_forward(move), v3_fl.size()=(" << v3_fl.size() << ")\n";
	cout << "\n";

	vector<int> v3_ff = get_vector_randoms(20);
	cout << "f_steal_forward:\n";
	f_steal_forward(move(v3_ff));
	cout << "f_steal_forward(move): v3_ff=(" << v3_ff.size() << ")\n";
	cout << "\n";

	vector<int> v2 = get_vector_randoms(20);
	cout << "l_islvalue_forward, ";
	l_islvalue_forward(v2);
	cout << "f_islvalue_forward, ";
	f_islvalue_forward(v2);
	vector<int> v3_m = get_vector_randoms(20);
	cout << "l_islvalue_forward(move), ";
	l_islvalue_forward(move(v3_m));
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
	(void) l_islvalue_forward;
	return 0;
}

//	Summary:
//		'std::move()' performs an unconditional cast to an rvalue
//		'std::forward()' performs a cast to rvalue <only> if <(the value given as argument is bound to an rvalue?)> <(the value given as argument was initalized as an rvalue?)>

