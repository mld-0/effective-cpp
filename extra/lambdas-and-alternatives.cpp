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


//	LINK: https://en.cppreference.com/w/cpp/language/lambda
//	{{{
//	Forms of a lambda:
//	1		[ captures ] ( params ) <(specs)> <(requires(optional))> { body }
//	2		[ captures ] { body }																			<C++23>
//	2		[ captures ] <(specs)> { body }																	<C++23>
//	3		[ captures ] <tparams> <(requires)> (params) <(specs)> <(requires(optional))>  { body }			<C++20>
//	4		[ captures ] <tparams> <(requires(optional))> { body }							<since-C++20, until-C++23>
//	4		[ captures ] <tparams> <(requires(optional))> <(specs)> { body }								<C++23>
//		1	Full form
//		2	Omitted parameters list
//		3	Same as 1, but specifies a generic lambda and explicitly proves a list of template parameters
//		4	Same as 2, but specifies a generic lambda and explicitly proves a list of template parameters
//	captures: comma separated list of zero/more captures, (optionally) beginning with <(capture-default?)>
//				can use variable without capturing it if: 
//						it is non-local, static, or thread local storage duration *1
//						it is a reference that has been initialized with a constant-expression
//				can read value without capturing it if:
//						has const non-volatile integral/enumeration type, and has been initialized by constant-expression
//						is constexpr and has no mutable members
//				*1	(in which case the variable cannot be captured)
//	tparams: non-empty comma-separated list of template parameters 
//				<(provide names to the template parameters of a generic lambda, see 'ClosureType::operator()')>
//	params: list of parameters (as in named function)
//	specs: <(specifiers, exception, attr, and trailing-return-type (in order) (each being optional))>
//	specifiers: if not provided, objects capture by copy are const in lambda body
//				mutable, allows body to modify objects called by copy and to call their non-const member functions
//				constexpr, function-call-operator/operator-template-specialization is a constexpr function
//				consteval, function-call-operator/operator-template-specialization is an immediate function
//				(contexpr/consteval are mutually exclusive)
//	exception: provide dynamic-exception-specification, (or <pre-C++20> the 'noexcept' specifier
//	attr: attribute-specification(?) <(for the type of the function-call-operator/operator-template of the closure type (any attribute specified does not appertain to the function call operator or operator template itself, but its type?))>
//	trailing-return-type: <(ret, where ret specifies the return type. If trailing-return-type is not present, the return type of the closure's operator() is deduced from return statements as if for a function whose return type is declared auto)>
//	requires: <since-C++20> add constraints(?) to the 'operator()' of the closure type
//	body: function body

//	<(If 'auto' is used as a type of a parameter (or <since-C++20> an-explicit-template-parameter-list(?) is provided), the lambda is a generic-lambda)>

//	<(Dangling references: if a non-reference entity is captured by reference (explicitly or implicitly), and the function call operator or a specialization of the function call operator template of the closure object is invoked after the entity's lifetime has ended, undefined behavior occurs. The C++ closures do not extend the lifetimes of objects captured by reference (Same applies to the lifetime of the current *this object captured via this))>

//	Lambda-capture: 
//	{{{
//		<>
//	1) simple by-copy capture
//	2) simple by-copy capture that is a pack expansion
//	3) by-copy capture with an initializer
//	4) simple by-reference capture
//	5) simple by-reference capture that is a pack expansion
//	6) by-reference capture with an initializer
//	7) simple by-reference capture of the current object
//	8) simple by-copy capture of the current object
//	9) by-copy capture with an initializer that is a pack expansion
//	10) by-reference capture with an initializer that is a pack expansion
//	}}}


//	Ongoing: 2022-05-04T03:49:44AEST 'cppreference' provides extensive/exhorbitant/unhelpfully-detailed details on the lambda -> better results from (starting with) friendlier <reference/source>-material?
//	Ongoing: 2022-05-04T03:37:54AEST lambdas, (problems?) calling non-const member functions of captured objects(?)
//	Ongoing: 2022-05-04T03:40:53AEST (a lot of) words above used above (without providing the <C++> definition) (i.e: attribute-specification, operator-template-specialization, dynamic-exception-specification, <...>) 
//	Ongoing: 2022-05-04T03:45:35AEST later definitions (see above) copied, (their) <meaning/definition> still unclear
//	}}}


//	LINK: https://docs.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170
//	{{{
//	Parts of a Lambda:
//	[=] () mutable throw() -> int { ... }
//		1)	'[=]' 			capture-clause (or lambda-introducer)
//		2)	'()' 			parameter-list (optional) (or lambda-declarator)
//		3)	'mutable' 		mutable-specification (optional)
//		4)	'throw()' 		exception-specialization
//		5)	'-> int'		trailing-return-type (optional)
//		6)	{ ... }			lambda-body

//	capture clause:
//	<>

//	generalized capture <C++14>:
//	<>

//	parameter-list: 
//	<>

//	mutable-specification:
//	<>

//	exception-specification:
//	<>

//	return-type:
//	<>

//	lambda-body:
//	<>

//	constexpr lambda expressions:
//	<>

//	<(Microsoft specific: lambdas are not supported by 'ref class', 'ref struct', 'value class', or 'value struct')>
//	}}}


//	Ongoing: 2022-05-04T04:00:59AEST continue through links until a complete picture of lambdas appears?
//	LINK: https://stackoverflow.com/questions/7627098/what-is-a-lambda-expression-in-c11
//	{{{
//	}}}
//	LINK: https://www.geeksforgeeks.org/lambda-expression-in-c/
//	{{{
//	}}}
//	LINK: https://towardsdatascience.com/c-basics-understanding-lambda-7df00705fa48
//	{{{
//	}}}
//	LINK: https://www.cprogramming.com/c++11/c++11-lambda-closures.html
//	{{{
//	}}}
//	LINK: https://www.softwaretestinghelp.com/lambdas-in-cpp/
//	{{{
//	}}}


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

