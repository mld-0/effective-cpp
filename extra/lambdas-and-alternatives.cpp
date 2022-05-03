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
//	Ongoings:
//	{{{
//	Ongoing: 2022-05-04T02:14:22AEST extra/stl-algorithms, (an item for) STL '_if' algorithms (find_if, remove_if, count_if) (and alike), (use with 'most trivial predicates' (for their basic functionality), and with lambdas for expanded <flexibility/capability>)
//	}}}

//	(Book claim:) The functionality of the C++ lambda can be accomplished without it.
//	Lambdas provide a convenient way to create function objects, (and make C++ a better programming language).

//	Lambdas provide a convenient way to <create/supply> smartpointer custom deleters.
//	Basic use of the STL '_if' algorithms (find_if, remove_if, count_if) <(which take a 'begin()' pointer, and 'end()' pointer, and a conditional function?)> is typically with a trivial predicate (simple boolean function), <(while lambdas provide a powerful tool for expanded <flexibility/capability>?)>
//	<(Lambdas provide a way to specify predicates for condition variables in the threading API)>


//	lambda expression: an expression, i.e: 
//	<(<Literal form>, passable as argument without needing to assign to variable)>
//		[](int val) { return 0 < val && val < 10; }

//	closure: runtime object created by a lambda.
//	Depending on capture mode, holds copies-of/references-to captured data.
//		or can be assigned to a variable object 
//	<(possible to have multiple closures of a closure type corresponding to a single lambda?)>

//	closure class: <(class from which a closure is instantiated?)>
//	<(Each lambda causes the compiler to generate a unique closure class)>
//	Ongoing: 2022-05-04T02:37:15AEST meaning/example of/for closure class?


//	Lambda syntax:
//	<>


//	Capture modes: <C++11> by-reference and by-value
//	<>


//	<(Lambda Alternatives:)>
//		std::function
//		<function pointers?>
//		<?>


//	Ongoing: 2022-05-04T02:39:16AEST (declaring a lambda with a (captured?) global variable), details of error 'does not have automatic storage duration'
//	<(error: 'x' cannot be captured because it does not have automatic storage duration)>
//int x = 23;
//auto c1 = [x](int y) { return x * y > 55; };


int main()
{

	//	<(assigning lambda expressions to various type variables:)> <(does this belong outside 'main'?)>
	//	auto:
	int x = 23;
	auto c1 = [x](int y) { return x * y > 55; };			
	//	typeof(c1):
		//	YCM: T = 'class(lambda)'
		//	get_type_name: T = 'lambda at lambdas-and-function-pointers.cpp:87:12'
	//	Ongoings:
	//	{{{
	//	Ongoing: 2022-05-04T02:43:50AEST (the problem of deducing 'auto' as if auto had been 'T'?)
	//	Ongoing: 2022-05-04T02:42:14AEST YCM GetType: auto = 'class(lamba)', c1 = '[x](int y) {};'
	//	Ongoing: 2022-05-04T02:32:42AEST some C++ get-type method which can get more details about 'lambda' than just 'lambda' [...] 'YcmCompleter GetType' does, (use it on 'c1' not on 'auto')
	//	Ongoing: 2022-05-04T02:36:12AEST (some meaning <to?> (the significance of) a copy of a lambda?)
	//	}}}

	cout << "get_type_name<decltype(c1)>()=(" << get_type_name<decltype(c1)>() << ")\n";

	return 0;
}

