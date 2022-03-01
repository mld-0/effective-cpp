//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
class Widget;
//	Ongoing: 2022-03-02T02:24:26AEDT document should be 'move-vs-forward' (<why> is git so bad with renaming files?)

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

//	Move Semantics: makes it possible to replace expensive copies with cheaper moves. The move-ctor and move-assignment member functions control the semantics of moving.  <(<Default/implicit> class support for move?)> <(Move only types (are those with deleted copy ctor/assignment?))>
//	Perfect Forwarding: makes it possible to write function templates that take arbitrary arguments and forward them to a third function as they were recieved.
//	Unless a type supports move assignment/construction, a move is just as copy.

//	TODO: 2022-03-02T02:24:05AEDT effective-c++, item 23, move-forward, review implicit move assignment/ctor rules (having copied the ctor rules to be cleaned up, along with a statement of presumption that implicit move assignment works the same way?)
//	Default move assignment/constructor behaviour:
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


//	rvalue references are the language feature that makes both features possible. 
//	The construct 'T&&' does not always represent an rvalue reference.
//	A parameter <function argument/*1> is always an lvalue, even when it has the type rvalue reference. That is:
//	<(*1 (or thing-like-a-function-argument?) what is the definition of a C++ 'parameter'?)>
void f(Widget&& w_arg); 
//	'w_arg' is an lvalue <(what is the (meaning of the) address of 'w_arg'?)>

//	'std::move()' and 'std::forward()' are function templates that perform casts.

//	Example: approximation of 'std::move()', casts 'param' to an rvalue (for reference collapsing see item 28)
//	The result of applying move to an object is that it is cast to an rvalue
template<typename T>
decltype(auto) my_move(T&& param) {	
	using RT = typename remove_reference<T>::type&&;
	return static_cast<RT>(param);
}


//	Example: move() doesn't necessarily actually move, <(it may under some circumstance perform a copy (const, are there others?))>
//	Ongoing: 2022-02-26T01:57:23AEDT class 'Annotation', moving a 'const string'
//	'move' calls on const objects are silently transformed into copy operations
//	Whenever one calls 'move' or 'forward' when passing a variable, one must assume that its value may have been erased by the callee.


//	Example: approximation of std::forward()'
//	'forward' is a conditional version of 'move', it performs a move (cast to rvalue) only if its argument was initalized with an rvalue (recall that parameters are lvalues)
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}
template <class T>
inline T&& my_forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value,
                  "Can not forward an rvalue as an lvalue.");
    return static_cast<T&&>(t);
}

//	Ongoing: 2022-02-26T02:02:20AEDT auto-initialization from move/forward?
//	Ongoing: 2022-02-26T02:04:24AEDT implementing 'move(var_str)' as 'forward<string>(var_str)'? (see example Widget move ctor)

int main()
{
	//	Ongoing: 2022-02-26T23:13:49AEDT need examples of use of move/forward (since I find myself unable to call forward()? -> because forward must be used as 'forward<T>(x)'?)

	return 0;
}

//	Summary:
//		'std::move()' performs an unconditional cast to an rvalue
//		'std::forward()' performs a cast to rvalue only if the argument is bound to an rvalue

