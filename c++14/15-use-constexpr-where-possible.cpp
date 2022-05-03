//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <array>
using namespace std;
//	{{{2
//	Ongoing: 2022-03-28T01:09:46AEDT (as used on variables) constexpr vs const
//	Ongoing: 2022-03-28T01:16:44AEDT constexpr function side effects? (is this at all related to the question of decltype() (was it?) side effects?)
//	{{{
//	TODO: 2022-03-28T23:20:49AEDT effective-c++, item 15, use constexpr where possible, (function) is_constexpr() (or) <meta-template> is_constexpr<>()
//	}}}

//	Use constexpr where possible. This shifts work from run-time to compile-time, and produces results that can be used in a larger range of contexts that non-constexpr values/functions.

//	constexpr was introduced with <C++11>. It has a different meaning when applied to functions vs to objects.
//	constexpr variables have a (const) value that is known at link time. Such values are privileged, they <may> be placed in read only memory. 
//	constexpr integral values can be used in contexts where an 'integral constant expression' is required, such as: array sizes, template arguments, enumerator values, <((what are?) alignment specifiers)>.
//	All constexpr objects are const, not all const objects are constexpr. constexpr offers a stronger guarentee that the value will known at compile time (something not necessarily the case for const).


//	Example: If a variable is declared constexpr, the compiler will insist its value be known at compile time.
int uinit1;	
//constexpr auto cuinit1 = uinit1;				//	error, value must be initalized by constant expression


//	Example: to declare template, value must be const/constexpr
int isize1 = 10;
//array<int, isize1> a2;						//	error, template argument is not a constant expression
constexpr auto csize1 = 10;
array<int, csize1> a1;
const auto csize2 = 5;
array<int, csize2> a2;


//	A constexpr function produces a compile time constant when it is called with a compile time constant, that is, the function will be evaluated for that value at compile time, and the result used in place of that function call. If a constexpr function throws an exception, calls any non-constexpr function, or modifies any variable visible outside the function, the result is no longer constexpr. 
//	<(book claim, mixed known/uknown at compile time arguments to constexpr function will be rejected? what does rejected mean -> rejected for compile time treatment?)>

//	Ongoing: 2022-03-28T01:38:42AEDT is noexcept actually necessary for a constexpr function?
//	Ongoing: 2022-03-28T01:20:43AEDT what if I throw an exception? 
//	Ongoing: 2022-03-28T01:20:09AEDT significance of noexcept for constexpr 'pow()'?
//	<C++11> and <C++14> place different restrictions on constexpr functions:
//	In <C++11> constexpr functions are limited to a single executable statement, a 'return'. 
//	This necessitates use of both the '? :' operator, recursion
//	Example: <C++11> constexpr pow() 
constexpr int pow_cpp11(int base, int exp) noexcept {
	return (exp == 0 ? 1 : base * pow_cpp11(base, exp-1));
}
//	Example: <C++14> constexpr pow() 
constexpr int pow(int base, int exp) {
	//	If a constexpr hits an exception, (or exit()), the result is not a constexpr. Note that the error message generated does not include the exception itself, but describes a failure to produce a constexpr.
	if (exp < 0) { throw invalid_argument("exp < 0 invalid"); }	
	if (exp == 1) { return 1; }
	int result = base;
	for (int i = 1; i < exp; ++i) { result *= base; }
	return result;
}

//	can use 'pow()' in places where <constexpr/const> expected
constexpr auto csize3 = pow(2,10);
array<int, csize3> a3;
array<int, pow(3,5)> a4;

//	constexpr are limited to taking are returning literal-types, that is, all types that can have evalues determined during compilation. For a type to be literal (see below):
//	LINK: https://en.cppreference.com/w/cpp/named_req/LiteralType
//	{{{
//		possibly cv-qualified void (so that constexpr functions can return void); (since C++14)
//		scalar type;
//		reference type;
//		an array of literal type;
//		possibly cv-qualified class type that has all of the following properties:
//			has a trivial (until C++20)constexpr (since C++20) destructor,
//			is one of
//			an aggregate type,
//			a type with at least one constexpr (possibly template) constructor that is not a copy or move constructor,
//			a closure type
//			(since C++17)
//			for unions, at least one non-static data member is of non-volatile literal type,
//			for non-unions, all non-static data members and base classes are of non-volatile literal types.
//	}}}

//	<(In <C++11>, all build-in(?) <(does that mean primative? what about string, vector, ect?)> types except void qualify)> [...] -> vector is not literal, string is not literal, <>
//constexpr vector<int> v1 = {2,3,4};							//	error, not literal type
//constexpr string s1 = "asdf";									//	error, not literal type

//	Example: 'Point' literal type, constexpr ctor (and other member functions)
//	<(Member variables cannot be constexpr)>
//	Use of static variables is non-constexpr
//	<(Rules: ctor must be constexpr if it exists, dtor must be constexpr if it exists?)>
class Point {
public:
	constexpr Point(double X=0, double Y=0) noexcept 
		: x(X), y(Y) 
	{}
	constexpr double getX() const noexcept { return x; }
	constexpr double getY() const noexcept { return y; }
	//	Ongoing: 2022-03-29T00:15:38AEDT is it meaningful to declare (besides ctor/dtor) a non-const member function constexpr? ((what about a) static constexpr function?)
	//	Ongoing: 2022-03-28T01:43:21AEDT (what if I) declare setX/setY constexpr? <(compiler doesn't say no?)> <(what would that even mean?)> [...] -> can a member function be constexpr and not const? -> can declare member function constexpr, but can't use it on a constexpr variable unless it is const? [...] <- *should* we ever declare a non-const member function constexpr?
	void setX(double X) noexcept { x = X; }
	void setY(double Y) noexcept { y = Y; }
	////	constexpr functions declared 'const' (can't be) -> not callable on constexpr objects (since those are const)
	//constexpr void setX(double X) noexcept { x = X; }
	//constexpr void setY(double Y) noexcept { y = Y; }
private:
	double x, y;
};


//	Example: An empty class is a literal type
class Empty {};


//	Ongoing: 2022-03-29T00:24:13AEDT { x, y } implicitly converts to Point(x,y)? 
//	Example: contexpr function recieving/returning custom literal type 'Point'
constexpr Point midpoint(const Point& p1, const Point& p2) noexcept {
	auto x = (p1.getX() + p2.getX()) / 2;
	auto y = (p1.getY() + p2.getY()) / 2;
	return { x, y };
}

constexpr Point reflection_i(const Point& p) noexcept {
	return { -p.getX(), -p.getY() };
}
constexpr Point reflection_ii(const Point& p) noexcept {
	Point result;
	//	Ongoing: 2022-03-29T00:30:32AEDT a case where a non-const constexpr member function is meaningful, when we are calling it on a temporary non-constexpr object which will be return as a constexpr value
	result.setX(-1 * p.getX());
	result.setY(-1 * p.getY());
	return result;
}


//	TODO: 2022-03-28T23:38:25AEDT effective-c++, item 15, use constexpr where possible, (how) does 'is_constexpr()' work?
//	Ongoings:
//	{{{
//	Ongoing: 2022-03-28T23:40:05AEDT checking something is constexpr -> can one assign it to 'constexpr auto x = '?
//	Ongoing: 2022-03-28T23:34:10AEDT but (how) does it work?
//	Ongoing: 2022-03-28T23:25:16AEDT meaning of '...' as function/template-function argument -> (variadic but with no type/parameter?)
//	LINK: https://stackoverflow.com/questions/55288555/c-check-if-statement-can-be-evaluated-constexpr
//	}}}
//	Example: is_constexpr()
//	This solution leverages that (1) lambda expressions can be constexpr as of C++17 (2) the type of a captureless lambda is default constructible as of C++20. The idea is, the overload that returns true is selected when and only when Lambda{}() can appear within a template argument, which effectively requires the lambda invocation to be a constant expression.
template<class Lambda, int=(Lambda{}(), 0)>
constexpr bool is_constexpr(Lambda) { return true; }
constexpr bool is_constexpr(...) { return false; }


//	Example: constexpr, side effects <(cannot occur, since a function is no longer constexpr if it modifies variables visible outside the function (practically by definition preventing side effects?) (do not tell me there is not some way to trip up constexpr vis-a-vis side effects?))>
//	<>


//	Example: constexpr and noexcept(?) <(necessary/anything more than good practice?)>
//	<>


void f() {}

int main()
{
	//	Example: If an exception is reached, the result is not a constexpr
	//constexpr auto a = pow(2, -3);						//	error, result is not constexpr

	//	Example: constexpr function is not evaluated as at compile time because result is not used as a constexpr
	//pow(2, -3);											//	generates exception at runtime

	constexpr int x = 5;
	constexpr auto a = pow(x, 3);
	cout << "a=(" << a << ")\n";

	cout << "pow(2,10)=(" << pow(2, 10) << ")\n";
	cout << "\n";

	//	Example: literal types can be declared constexpr
	constexpr Point p1;
	constexpr Empty e1;
	//p1.setX(5);											//	error, function is not marked constexpr


	//	Ongoing: 2022-03-29T00:05:16AEDT 'is_literal_type' is deprecated -> (removed in C++20?) (what is the replacement?) -> can we assert that (a constexpr of) something is uncompilable?
	//cout << is_literal_type<Point>() << "\n";
	//cout << is_literal_type<string>() << "\n";


	//	Ongoing: 2022-03-29T00:12:59AEDT is_constexpr does what / is used how?
	//cout << is_constexpr(ci1) << "\n";
	//cout << is_constexpr(cei1) << "\n";
	//cout << is_constexpr(pow(2,10)) << "\n";
	//cout << is_constexpr(csize3) << "\n";

	//	Use of 'midpoint' constexpr function
	constexpr Point cp1(3,2);
	constexpr Point cp2(5,7);
	constexpr Point cp3 = midpoint(cp1, cp2);
	constexpr Point cp4 = reflection_i(cp3);

	(void) e1; (void) p1; (void) cp3; (void) cp4;
	return 0;
}

//	Summary:
//		constexpr objects are const, and are initalized with values known at compile-time.
//		constexpr functions produce constexpr results when called with constexpr arguments.
//		constexpr values can be used in contexts like template parameters and primative array sizes.
//		constexpr is part of an object or function interface. Removing it make break client code.

