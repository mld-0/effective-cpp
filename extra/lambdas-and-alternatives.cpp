//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <type_traits>
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
//	Ongoing: 2022-05-04T21:13:15AEST default capture (by-reference) (also somehow dependent-on/captures values (as they were) when the lambda is defined?)
//	Ongoing: 2022-05-04T21:12:49AEST default capture (by-value) (also) captures the values of variables as of when the lambda is defined?
//	Ongoing: 2022-05-04T02:14:22AEST extra/stl-algorithms, (an item for) STL '_if' algorithms (find_if, remove_if, count_if) (and alike), (use with 'most trivial predicates' (for their basic functionality), and with lambdas for expanded <flexibility/capability>)
//	Ongoing: 2022-05-04T20:48:28AEST capturing variables vs passing parameters (arguments) (reasons, best practice)?
//	Ongoing: 2022-05-04T20:50:45AEST some wider meaning of 'closure' (beyond an instance of a lambda?)
//	}}}

//	(Book claim:) The functionality of the C++ lambda can be accomplished without it.
//	Lambdas provide a convenient way to create function objects, (and make C++ a better programming language).

//	Lambdas provide a convenient way to <create/supply> smartpointer custom deleters. <(Lambdas provide a way to specify predicates for condition variables in the threading API)>.
//	Lambdas are used to encapsulate a few lines, for passing to an algorithm / asynchronous-function.
//	Basic use of the STL '_if' algorithms (find_if, remove_if, count_if) <(which take a 'begin()' pointer, and 'end()' pointer, and a conditional function?)> is typically with a trivial predicate (simple boolean function), <(while lambdas provide a powerful tool for expanded <flexibility/capability>?)>
	


//	lambda expression: an expression, <(<Literal form>, passable as argument without needing to assign to variable)>
//		[ captureClause ] ( parameters ) -> returnType { body };
//	Example:
//		[](int val) { return 0 < val && val < 10; }

//	Lambdas cannot be overloaded

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
//		A lambda can access (capture) variables from the surrounding scope. The capture clause specifies these variables, and whether they are captured by-value or by-reference '&'. <(An empty capture clause '[]' indicates the lambda accesses no variables in the enclosing scope?)>. 
//	capture-default mode: 
//		[&] <(all variables referred to are captured by reference)> 
//		[=] <(all variables referred to are captured by value)>
//	Ongoing: 2022-05-04T19:11:45AEST '[&]' / '[=]' mean any (available) variable referred to in the lambda is captured(?)
//	Rules:
//	{{{
//	[&, i]			//	Ok
//	[&, &i]			//	Error, 'i' preceded by & when '&' is the default
//	[=, this]		//	Error, 'this' (without '*') when '=' is the default
//	[=, *this]		//	Ok, captures 'this' by-value
//	[i, i]			//	Error, 'i' repeated
//	}}}
//	To use lambda expressions in the body of a class member function, pass the this pointer to the capture clause to provide access to the member functions and data members of the enclosing class.
//	variadic template: <>
//		[args...]
//	Reference captures introduce a lifetime dependency, but value captures have no lifetime dependencies. It's especially important when the lambda runs asynchronously. If you capture a local by reference in an async lambda, that local could easily be gone by the time the lambda runs. Your code could cause an access violation at run time.

//	generalized capture <C++14>: introduce/initialize new variables in the capture clause 
//		auto a = [ptr = move(pNums)]() {};

//	parameter-list: (optional), resembles parameter (argument) list for a function
//		auto y = [](int first, int second) {};
//	<(If the parameter type is generic, 'auto' can be used as the type - telling the compiler to create the function call operator as a template (each instance of 'auto' is equivalent to a distinct type parameter))>
//		auto y = [](auto first, auto second) {};
//	An empty parameter list '()' can be omitted if the lambda doesn't contain exception-specification, trailing-return-type, or mutable.

//	mutable-specification: <(allows a lambda to modify variables that are captured by value)>
//	<(a lambda's function call operator is const-by-value if the lambda is not declared mutable)>
//	Modifying variables captured by-value modifies the copy specific to the lambda, not the original variable.

//	exception-specification: Use 'noexcept' to indicate a lambda does not throw exceptions
//	Ongoing: 2022-05-04T19:27:20AEST lambda exception-specification, <old-C++> use of 'throw()'?

//	return-type: automatically deduced ('auto' does not have to be specified unless using a trailing return type).
//	Not needed if lambda contains no more than 1 return statement. 
//		auto x1 = [](int i) { return i; };			//	Return type is 'int'
//		auto x2 = []{ return {1,2}; };				//	Error, can't deduce return type from initializer list
//		auto x3 = [](int i) -> int { return i; }; 	//	Trailing return type 'int'

//	lambda-body: compound statement, can contain anything allowed in the body of an ordinary function.
//	Can access: captured variables, parameters (arguments), locally declared variables, any variable with static storage duration i.e: global variables, <(class data members when declared inside class and 'this' is captured)>

//	Example: explicitly capture 'n' by value, and implicitly capture 'm' by reference
//		int m = 0;
//		int n = 0;
//		[&, n] (int a) mutable { m = ++n + a; }(4);
//	'mutable' allows 'n' (captured by-value) to be modified within the lambda (note: this is modification of a copy, and does not change the outside value).

//	constexpr lambda expressions: like a constexpr function, a lambda declared constexpr produces a constexpr result when called constexpr values <(and no captured data is not constexpr?)>
//	An explicitly/implicitly constexpr lambda converted to a function pointer produces a constexpr function <(only a capture-less lambda can be implicitly converted to a function pointer?)>
//	Example: A lambda is implicitly constexpr if it satisfies the requirements of a constexpr function
auto add_32 = [](int n) { return 32+n; };
constexpr int example_constexpr_result = add_32(53);


//	A lambda expression can only capture variables that have automatic storage duration. However, you can use variables that have static storage duration in the body of a lambda expression.
//	A lambda can be an argument-to / return-type-of another lambda.
//	<(Microsoft specific: lambdas are not supported by 'ref class', 'ref struct', 'value class', or 'value struct')>
//	}}}


//	Ongoing: 2022-05-04T04:00:59AEST continue through links until a complete picture of lambdas appears?
//	LINK: https://stackoverflow.com/questions/7627098/what-is-a-lambda-expression-in-c11
//	{{{
//	Lambdas provide inline anonymous functions 

//	<C++03> technique: (use of 'for_each')
//	{{{
//		namespace {
//		  struct f {
//		    void operator()(int) {
//		      // do something
//		    }
//		  };
//		}
//		void func(std::vector<int>& v) {
//		  f f;
//		  std::for_each(v.begin(), v.end(), f);
//		}
//	<(The problem? (This (see below) is not allowed?))>:
//		void func2(std::vector<int>& v) {
//		  struct {
//		    void operator()(int) {
//		       // do something
//		    }
//		  } f;
//		  std::for_each(v.begin(), v.end(), f);
//		}
//	}}}
//	The <C++11> solution (to the problem (see above): an inline anonymous functor to replace 'struct f'
void func3(vector<int>& v) {
	for_each(v.begin(), v.end(), [](int) { /* do something */ });
}
void func4(std::vector<double>& v) {
	std::transform(v.begin(), v.end(), v.begin(),
	[](double d) -> double {
		if (d < 0.0001) {
			return 0;
		} else {
			return d;
		}
	});
}
//	Ongoing: 2022-05-04T21:00:40AEST most beautiful/correct way to write 'func4'?

//	Return type: deduced by compiler where possible. Use '-> T' to specify trailing return type where deduction is not possible.

//	Capturing variables:
//	<(the value of (by-value) capture values are set when the lambda is defined)>
//		[&epsilon, zeta] 		captures epsilon by reference and zeta by value
//		[&] 					captures all variables used in the lambda by reference
//		[=] 					captures all variables used in the lambda by value
//		[&, epsilon] 			captures all variables used in the lambda by reference but captures epsilon by value
//		[=, &epsilon] 			captures all variables used in the lambda by value but captures epsilon by reference
//	The generated operator() is const by default, with the implication that captures will be const when you access them by default. This has the effect that each call with the same input would produce the same result, however you can mark the lambda as mutable to request that the operator() that is produced is not const.

//	<(default capture?)>
//		[=](int arg1){} 		// capture all not-static vars by value
//		[&](int arg1){} 		// capture all not-static vars by reference

//	Initialized Lambda Captures:
//	An element of the capture list can now be initialized with '='. This allows renaming of variables and to capture by moving. <(Types are automatically deduced (using 'auto' rules?))>
//	An example taken from the standard:
//		int x = 4;
//		auto y = [&r = x, x = x+1]()->int {
//		            r += 2;
//		            return x+2;
//		         }();  // Updates ::x to 6, and initializes y to 7.
//	Example: capture with 'move()'
//		auto ptr = std::make_unique<int>(10); 
//		auto lambda = [ptr = std::move(ptr)] { return *ptr; };
//	Ongoing: 2022-05-04T21:32:57AEST meaning of '[&r = x, x = x+1]'(?) -> '&r' is an lvalue reference to x(?)


//	Generic Lambda:
//		auto lambda = [](auto x, auto y) {return x + y;};

//	Execute lambda immediately upon definition: (a powerful tool for refactoring complex functions)
//	equivalent:
//		[&](){ /* do something */ }();
//		{ /* do something */ }

//	}}}


//	<(Some of the?)> Core Guidelines on Lambdas:
//	LINK: https://www.modernescpp.com/index.php/c-core-guidelines-function-objects-and-lambas
//	{{{
//	Use a lambda when a function won’t do (to capture local variables, or to write a local function)
//	{{{
//		std::function<int(int)> makeLambda(int a){    // (1)
//		    return [a](int b){ return a + b; };
//		}
//		int main(){
//		    auto add5 = makeLambda(5);                // (2)
//		    auto add10 = makeLambda(10);              // (3)
//		    add5(10) == add10(5);                     // (4)
//		}
//	}}}
//	Prefer capturing by reference in lambdas that will be used locally, including passed to algorithms, (and conversely) Avoid capturing by reference in lambdas that will be used nonlocally, including returned, stored on the heap, or passed to another thread
//	{{{
//	For efficiency and correctness reasons, your lambda expression should capture its variables by reference if the lambda expression is locally used. Accordingly, if the lambda expression is not used locally, you should not capture the variables by reference but copy the arguments. If you break the last statement you will get undefined behavior.
//	Here is an example of undefined behavior with lambda expressions: (The undefined behavior is that the function makeLambda returns a lambda expression with a reference to the local variable local).
//		std::function<int(int)> makeLambda(int a){
//		    int local = 2 * a;
//		    auto lam = [&local](int b){ return local + b; };           // 1
//		    std::cout << "lam(5): "<<  lam(5) << std::endl;            // 2
//		    return lam;
//		}
//		int main(){
//		  std::cout << std::endl;
//		  int local = 10;
//		  auto addLocal = [&local](int b){ return local + b; };        // 3
//		  auto add10 = makeLambda(5);
//		  std::cout << "addLocal(5): " << addLocal(5) << std::endl;    // 4
//		  std::cout << "add10(5): " << add10(5) << std::endl;          // 5
//		  std::cout << std::endl;
//		}
//	}}}
//	Use lambdas for complex initialization, especially of const variables:
//	{{{
//		//	bad example: (not synchronized, not suitable for multithreading)
//		widget x;   // should be const, but:
//		for (auto i = 2; i <= N; ++i) {             // this could be some
//		    x += some_obj.do_something_with(i);  // arbitrarily long code
//		}                                        // needed to initialize x
// 		//	from here, x should be const, but we can't say so in code in this style
//	Conceptually, you only want to initialize widget x. If it is initialized it should stay constant. This is an idea we can not express in C++. If widget x is used in a multithreading program, you have to synchronize it. This synchronization would not be necessary if widget x was constant.

//		//	good example:
//		const widget x = [&]{
//		    widget val;                                // assume that widget has a default constructor
//		    for (auto i = 2; i <= N; ++i) {            // this could be some
//		        val += some_obj.do_something_with(i);  // arbitrarily long code
//		    }                                          // needed to initialize x
//		    return val;
//		}();
//	Thanks to the in-place executed lambda, you can define the widget x as a constant. You can not change its value and, therefore, you can use it in a multithreading program without expensive synchronization.
//	}}}
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


//	Lambdas that capture "this" should capture everything explicitly
//	LINK: https://rules.sonarsource.com/cpp/RSPEC-5019
//	{{{
//	A lambda can only capture local variables. When a lambda is defined within a member function, you may believe that you are capturing a member variable of the current class, but in fact, what you are capturing is this. This may be very surprising, and lead to bugs if the lambda is then used after the current object has been destroyed.
//	Therefore, it’s better to be explicit about exactly what is captured as soon as this is captured.
//	If the lambda is used immediately (for instance, called or passed as an argument to std::sort), there is no such risk and no issue is raised.
//	In C++20, capturing this via [=] has been deprecated. An issue is raised in that case, even if the lambda is used immediately.
//	Note: This rule does not apply if the capture list of the lambda contains *this (possible since C++17). In that situation, what is captured is not the pointer this, but a local copy of the object pointed-to by this and any reference to this (explicit or implicit) in the lambda body then refers to this local copy
//	Ongoing: 2022-05-04T21:05:55AEST does the book not cover this?
//	{{{
//	//	Noncompliant Code Example:
//	void useLambda(std::function<int,int> lambda);
//	
//	class A {
//	  int i;
//	  void f(int j) {
//	    auto l = [=](int k) { return i+j+k;}; // Noncompliant, someone reading the code might believe that i is captured by copy
//	    useLambda(l);
//	  }
//	};
//	//	Compliant Solution:
//	void useLambda(std::function<int,int> lambda);
//	
//	class A {
//	  int i;
//	  void f(int j) {
//	    auto l = [this, j](int k) { return i+j+k;}; // It is now clearer that i is not directly captured
//	    useLambda(l);
//	    // auto l = [i, j](int k) { return i+j+k;}; // Would not compile
//	
//	    auto l2 = [=, *this](int k) { return i+j+k;}; // Compliant, i refers to the member i of the captured copy
//	    useLambda(l2);
//	
//	    auto l3 = [=](int k) { return i+j+k;}; // Compliant because l3 is only used immediately
//	    int ijk = l3(i,j,k);
//	  }
//	};
//	}}}
//	}}}
//	Core Guideline: If you capture this, capture all variables explicitly (no default capture)
//	LINK: https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#f54-if-you-capture-this-capture-all-variables-explicitly-no-default-capture
//	{{{
//	Reason
//	It's confusing. Writing [=] in a member function appears to capture by value, but actually captures data members by reference because it actually captures the invisible this pointer by value. If you meant to do that, write this explicitly.
//	
//	Example
//	class My_class {
//	    int x = 0;
//	    // ...
//	
//	    void f() {
//	        int i = 0;
//	        // ...
//	
//	        auto lambda = [=]{ use(i, x); };   // BAD: "looks like" copy/value capture
//	        // [&] has identical semantics and copies the this pointer under the current rules
//	        // [=,this] and [&,this] are not much better, and confusing
//	
//	        x = 42;
//	        lambda(); // calls use(0, 42);
//	        x = 43;
//	        lambda(); // calls use(0, 43);
//	
//	        // ...
//	
//	        auto lambda2 = [i, this]{ use(i, x); }; // ok, most explicit and least confusing
//	
//	        // ...
//	    }
//	};
//	Note
//	This is under active discussion in standardization, and may be addressed in a future version of the standard by adding a new capture mode or possibly adjusting the meaning of [=]. For now, just be explicit.
//	}}}

//	Intuition for by-value capture values being const by default: calling lambda with same arguments <should/will> produce the same result each time (not true for by-reference capture variables, which can be modified by a non-mutable lambda).

//	<(Lambdas can use <?> variables without having to capture them?)>


//	Capture modes: <C++11> by-reference and by-value
//	<>


//	<(Lambda expression / closure / nested lambda)>:
//	<>


//	<(Lambda Alternatives)>:
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
	cout << "\n";

	//	Example: cannot modify variables captured by value without 'mutable' (copy is modified, original is not).
	int m = 0;
	int n = 0;
	//[&, n] (int a) mutable { m = 53; n = 27; }(4);
	//[&, n] (int a) { m = 53; n = 27; }(4);
	//	Ongoing: 2022-05-04T20:01:56AEST this is syntax (see above) to declare and call a lambda in a single statement?
	cout << "(m,n)=(" << m << "," << n << ")\n";
	cout << "\n";


	(void) example_constexpr_result;
	return 0;
}

