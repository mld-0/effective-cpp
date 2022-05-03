//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <ios>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
class Widget {};

//	LINK: https://www.learncpp.com/cpp-tutorial/exception-specifications-and-noexcept/

//	<C++98> promise not to emit an exception:
int f_i(int x) throw();

//	<C++11> promise not to emit an exception (better):
int f_ii(int x) noexcept;

//	Ongoings:
//	{{{
//	Ongoing: 2022-04-02T02:31:18AEDT <purpouse/when> one might declare a function 'noexcept(false)'(?) <(is no exception specification not equivalent to <this>, is the only purpouse it serves clarity (emphasising that we mean to (have the right to) throw exceptions) (and not have terminate() called immediately <indeterminately?> before/after (after meaning never) <> that dtors are called(?)))>
//	Ongoing: 2022-04-02T02:26:10AEDT is <C++98> 'throw(exception_type)' deprecated/removed by any past/current/future C++ version(?) [...] -> <C++17> commands that we replace this specification with 'noexcept(false)' instead
//	}}}
//int f_iii(int x) throw(logic_error);					//	error, <C++-17> forbids this old style, 
int f_iii(int x) noexcept(false);						//	use 'noexcept(false)'


//	Three guarantees of function exception safety: (pre-c++11/29-exception-safety)
//		The basic guarantee: if exception is thrown, the program remains in a valid state and no memory is leaked
//		The strong guarantee: if exception is thrown, the state of the program is unchanged (atomic function)
//		The nothrow guarantee: a promise to never throw exceptions for an operation that cannot fail
//	One should offer the strongest guarantee that is practical

//	<C++98> exception specifications require a summary of the exceptions a function might emit. Changes to implementation (adding new exceptions) requires changing this specification, which forms part of the function signature. Such changes could break client code. <C++98> exception specifications remain valid, but are deprecated.

//	<C++11> limits exception specifications to a 'noexcept' declaration, indicating that the function does not throw. This becomes part of the functions signature. (expect subsequent removal to break client code).
//	<(Callers can query the noexcept status of a function)>
//	'noexcept' allows important compiler optimizations (more so than 'throw()').



//	Which functions <are/should-be> noexcept/noexcept(false):
//		Functions that are potentially throwing (if not implicitly-declared or defaulted):
//				Normal functions (all user defined functions are implicitly 'noexcept(false)'), including:
//				User-defined constructors
//				User-defined operators
//		Functions that are non-throwing by default:
//				Destructors
//		Implicit/Default Functions are 'noexcept' where possible:
//				Constructors: default, copy, move
//				Assignments: copy, move
//				Comparison operators (as of C++20)
//				<(is this list current (as of) <C++20/+>?) [...] <- where is it from?)>
//		{{{
//	TODO: 2022-04-02T03:04:29AEDT effective-c++, item 14, declare noexcept, (continuing from) (item 17, default/implicit functions), clarify the (generally correct?) rule 'default/implict' functions are noexcept where possible (we are given the list: default/copy/move-ctors, copy/move-assignment, comparison, are there more 'default/implicit' functions than those (and are they also noexcept where possible) (i.e: an addition operator?), (?))
//		<(implicit-functions/just-those-above/these-are-the-same?)> are 'noexcept' where possible. That is, if any of the listed implicit/default functions call (explicitly or implicitly) another function which is potentially throwing, then the listed function will be treated as potentially throwing as well.)> 
//		Implicit functions <(ctors/dtors/operators/all-implicit-functions?/other-inherited-functions?)> are noexcept where possible <(that is, provided there are no data members which are 'noexcept(false)' for that same <would-be> implicit function)>. 
//		<(If any data member has an operator <copy/move-ctor/assign> <other operators? equality? (is there even an implicit) addition?> that is 'noexcept(false)', then that implicit operator function for the class is also 'noexcept(false)' [...] -> discuss more in (item 17, <special/implicit> member function generation))>.
//	Ongoing: 2022-04-02T02:57:34AEDT rules of 'noexcept' implicit/default function, to be cleaned-up (see below)
//	Ongoing: 2022-04-02T03:02:37AEDT <(are there more implicit/default functions than those below and are they not noexcept?)>
//		}}}

//	Best 'noexcept' Practice:
//		declare functions 'noexcept' to express an explicit no-fail or no-throw guarantee
//		strive to offer 'noexcept' where practical
//		If in doubt, do not declare 'noexcept' (it is easy to add and difficult to remove)
//		<(<necessisty/use-of> 'noexcept(false)' for user defined functions, (have we not already said only for clarity?))>


//	Ongoing: 2022-03-27T19:51:33AEDT is an explicit 'noexcept' required to <make use of a move/do other only-noexcept thing>, or is the compiler able to deduce whether the move in question is noexcept in some cases?

//	Many C++ functions, i.e: 'vector::push_back' / 'deque::insert' use move operations only if the <(move ctor/assignment)> is known not to emit exceptions. 

//	It is frequently desirable to declare 'swap()' as noexcept 

//	Provide noexcept implementations if there is a natural way to do so. Overcomplicating a function to try and provide a noexcept specification is often counterproductive.

//	By default, all memory deallocation functions and dtors are noexcept. A dtor is only not noexcept if a member/parent class has a dtor which is declared 'noexcept(false)'.

//	Ongoing: 2022-04-01T19:29:10AEDT ctor initalization list catch block? (is this an implicit try-catch around the ctor initalization list?)
//	<(dtor is not called if exception is thrown by ctor)>
//	<(exception is re-thrown if caught in ctor initalization list catch block)>

//	<(If a throw is caught then dtor and stack <unwinding/popping> occur. If the exception is not caught, stack <unwinding/popping> is not guaranteed)>

//	Stack unwinding: The process of calling dtors for automatic objects constructed on the path from a try block to the point where an exception is thrown. If a dtor <(or any other function(?))> throws an exception at any point during stack unwinding, 'terminate' is called.


//	Ongoing: 2022-03-27T20:02:07AEDT use a proper example of swap being done properly for 'conditionally noexcept' example
//	Example: conditionally noexcept
void my_swap_i(Widget& lhs, Widget& rhs) noexcept(noexcept(std::swap(lhs, rhs))) {
	using std::swap;
	swap(lhs, rhs);
}
//template<class T, size_t N>
//void my_swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b)));
//	<>


//	Wide and Narrow contracts:
//	<>


//	Because noexcept functions in-practice often rely on code lacking a noexcept guarantee, the compiler typically offers no help in identifying inconsistencies between function implementation and exception specification.
//	Example: 'doWork' has a noexcept specification, even thought it calls 'setup' and 'cleanup' which do not.
void setup() {}
void cleanup() {}
void doWork() noexcept {
	setup();
	cleanup();
}


//	What happens when a noexcept function throws an exception
//	When a noexcept function throws an exception, <(terminate is called, dtors <may/may-not> be called(?))>

//	noexcept, Stack-unwinding, and dtors (not being called)
//	<>


//	<(dtors not being called?)>
struct Cat { Cat() { cout << "Cat()\n"; } ~Cat() { cout << "~Cat()\n"; } };

//	throwing function, if uncaught, Cat dtor is <(never?)> called before application terminates
void processCat() {
	cout << "processCat:\n";
	Cat c1;
	throw logic_error("my_error");
}

//	throwing noexcept function, Cat dtor <(always? optimization dependent?)> called, terminates application even if caught
void processCat_noexcept() noexcept {
	cout << "processCat_noexcept:\n";
	Cat c1;
	throw logic_error("my_error");
}



//	Ongoing: 2022-04-01T00:26:44AEDT any (clean) way to declare 'default copy/move ctor, plus these things'?
//	Ongoing: 2022-04-01T00:27:30AEDT <('noexcept(false)' is equivalent to no exception specification?)> [...] -> dtors are noexcept unless 'noexcept(false)' is explicitly specifified
//	Ongoing: 2022-04-01T00:31:14AEDT <(default move-ctor is noexcept where possible(?) -> how to <check/tell>?)>


//	Example: std containers only use move if move-ctor is declared noexcept
struct A {
	A() noexcept { cout << "A()\n"; }
	A(const A& lhs) noexcept { cout << "A(copy)\n"; }
	A(A&& lhs) noexcept { cout << "A(move)\n"; }
};
struct B {
	B() { cout << "B()\n"; }
	B(const B& lhs) { cout << "B(copy)\n"; }
	B(B&& lhs) { cout << "B(move)\n"; }
};


//	Example: Difference between 'noexcept' and 'throw()'
//	<>


//	Example: noexcept/noexcept(false) implicit functions
//	<>


int main()
{
	//	Use of move vs copy when resizing vector
	int n = 3;
	vector<A> v1; vector<B> v2;
	cout << "create 'vector<A> v1'\n";
	for (int i = 0; i < n; ++i) { v1.emplace_back(); }
	cout << "create 'vector<B> v2'\n";
	for (int i = 0; i < n; ++i) { v2.emplace_back(); }
	cout << "\n";

	//	Example: Query noexcept-ness of A/B move-ctors
	cout << std::boolalpha << "noexcept(A(copy))=(" << noexcept(A(std::declval<A&>())) << ")\n";
	cout << std::boolalpha << "noexcept(A(move))=(" << noexcept(A(std::declval<A&&>())) << ")\n";
	cout << std::boolalpha << "noexcept(B(copy))=(" << noexcept(B(std::declval<B&>())) << ")\n";
	cout << std::boolalpha << "noexcept(B(move))=(" << noexcept(B(std::declval<B&&>())) << ")\n";
	cout << "\n";

	//	<(uncaught exceptions, regular function does not call Cat dtor, noexcept function does?)>
	//processCat();							//	uncaught exception, terminates, Cat dtor not called
	//processCat_noexcept();				//	uncaught exception, terminates, Cat dtor is called
	//	catching exception, Cat dtor is called before e is printed
	try { processCat(); } catch (std::exception& e) { cout << "e=(" << e.what() << ")\n"; }
	cout << "\n";

	//	catching exception, Cat dtor is called, program terminates before e is printed
	//try { processCat_noexcept(); } catch (std::exception& e) { cout << "e=(" << e.what() << ")\n"; }
	//cout << "\n";

	return 0;
}

//	Summary:
//		noexcept is part of a functions interface. It is easy to add and difficult to remove.
//		dtors and memory-allocation functions are noexcept by default.
//		<(noexcept(false) is equivalent to having no exception specification)>
//		noexcept(condition) allows conditional noexcept declarations
//		noexcept is valuable for move operations, swap, memory deallocation, and dtors
//		Most functions are exception-neutral rather than noexcept

