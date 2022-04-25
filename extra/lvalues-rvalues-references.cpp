#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	An lvalue represents an object (variable) that occupies some identifiable location in memory.
//	An rvalue is an object that is not an lvalue, (does not occupy some identifiable location in memory).
//	<(An lvalue is a variable whose address can be taken with '&'? Are there exceptions to this rule?)> 
//	<(The name 'lvalue' comes from left-hand-side of assignment equals? Lvalues can be assigned to if they are not const)>
//	<(implicit lvalue-to-rvalue conversion is possible for all values that aren't arrays, functions, or incomplete types)>
//	<(An lvalue of a non-function, non-array type 'T' can be converted to an rvalue [...] If 'T' is a non-class type, the type of the rvalue is the cv-unqualified version of 'T', otherwise the type of the rvalue is 'T'. ('cv-unqualified' means not const or volatile))>.
//	<(C++ Rvalue References Explained: another definition for 'lvalue's, something that has a name)>

//	'T&' denotes an lvalue reference of type T
//	'T&&' denotes an rvalue reference, or a a universal reference if T is a deduced type and not const/volatile (A universal references acts as an lvalue/rvalue-reference depending on the l/r-value-ness of the value which initialized it). <C++11>

//	Ongoing: 2022-03-02T03:00:48AEDT (see below) article 'C++ Rvalue References Explained'
//	LINK: http://thbecker.net/articles/rvalue_references/section_01.html

//	Meaning of the address of an rvalue reference: returns a pointer to the object (which may/may-not be temporary) that reference is bound to. (for an lvalue cast to an rvalue, this is the same as the address of the lvalue), <(for rvalues it is a temporary object?)>, <(please explain: we are taking the address of an rvalue-reference (rvalues are not rvalue references?))>

//	Meaning of a const rvalue reference: <(like a const-lvalue-ref, the lies being 1. rvalue-references not being const by default (especially those initalized from constants), and 2. that (the things at the end of) rvalue-references are different (in practice) from (the things at the end of) lvalue-references?)>

//	Meaning of casting an lvalue to rvalue (add to item 24?): (this being the definition of 'std::move'), 

//	When an object is passed using 'move()', one cannot assume the contents of the object are the same <valid?> afterwards (however one also cannot assume it will be empty/changed). <(one assumes the object will be in a valid state, either as if default-constructed, or with the value of the object with which it was swapped -> it is meaningless to ask for a 'guarantee' from 'move()', since the move behaviour is implemented on a per type basis)>.

//	TODO: 2022-03-02T02:52:02AEDT effective-c++, extra/lvalue-rvalue-references, details/definitions of (or just more about) lvalue/rvalue/universal references

//	Reference Collapsing: (see item 28)
//		A& &		A&
//		A&& &		A&
//		A& &&		A&
//		A&& &&		A&&


void print_rvalue_address(int&& x) { cout << "print_rvalue_address=(" << &x << ")\n"; }
void assign_to_rvalue(int&& x, int val) { x = val; }

//	TODO: 2022-02-27T02:39:28AEDT effective-c++, extra/lvalues-rvalues, (how to make work) 'assign_value(T&& x, T val)' when called with 'assign_value(var1,27)' (considering that 'assign_value(T&&, int val)' works (so it isn't a question of type deduction) ... (what is this behaviour?) (consider also that 'assign_value(move(var1),27)' does work) (and that assign_value(T&&, T&) does not work with or without 'move')

//	Example: For f(T&& x, T val), <(there is no way to call such a function with an lvalue for 'x', if x is an lvalue int, then decltype(x) = 'int&', but decltype(val) = 'int'?)>
template<typename T> void assign_urefT_i(T&& x, T val) { x = val; }	

//	Example: For f(T&& x, T val), <(as with 'assign_urefT_i', there is no way to call such a function with an lvalue for 'x', and also 'val' must not be an rvalue)>
template<typename T> void assign_urefT_ii(T&& x, T& val) { x = val; }

template<typename T> void assign_urefT_iii(T&& x, const T val) { x = val; }	
template<typename T> void assign_urefT_iv(T&& x, const T& val) { x = val; }	

//	Example: A cheat solution, no conflicting types
template<typename T> void assign_urefT_cheat(T&& x, int val) { x = val; }


//	Example: Test if a value is an lvalue/rvalue <(but does it?)>
template<typename T>
constexpr bool is_lvalue(T&& x) {
	return std::is_lvalue_reference<T>{};
}


//	TODO: 2022-03-02T01:50:47AEDT effective-c++, extra/lvalues-rvalues, lvalue vs rvalue (vs template rvalue?) overloads example function calls

int main()
{

	int var1;
	var1 = 4;
	//	'rval1' is an lvalue of type lvalue-reference
	int& rvar1 = var1;
	//	'rrefvar1' is an lvalue of type rvalue-reference
	int&& rrefvar1 = move(var1);
	//4 = var;				//	error, cannot assign to rvalue
	//(var + 1) = 4;		//	error, cannot assign to rvalue


	//	TODO: 2022-03-02T01:37:06AEDT effective-c++, extra/lvalues-rvalues, contrive a situation where 'x1' and 'x2' behave differently for 'int x1 = 5;' 'int&& x2 = 5;'
	//	Ongoing: 2022-03-02T00:45:43AEDT another example of forward not differentiating as expected -> another example to review after (item 25) 
	//	Example: <(I do not like this example, show me a difference between 'int' and 'int&&')>
	int x1 = 5;
	int&& x2 = 7;			//	x2 is an lvalue that has type rvalue-reference
	cout << "int x1 vs int&& x2:\n";
	cout << "is_lvalue(x1)=(" << is_lvalue(x1) << ")\n";
	cout << "is_lvalue(forward<int>(x1)=(" << is_lvalue(forward<int>(x1)) << ")\n";
	cout << "is_lvalue(x2)=(" << is_lvalue(x2) << ")\n";
	cout << "is_lvalue(forward<int>(x2))=(" << is_lvalue(forward<int>(x2)) << ")\n";
	cout << "\n";

	//	Example: const lvalues can be bound to rvalues
	const int& crefx2 = 5;


	//	Example: Conversion between lvalues and rvalues
	int a = 1;
	int b = 2;
	int c = a + b;		//	+ needs rvalues <(it does?)>, a/b are converted to rvalues, and an rvalue is returned

	//	Example: '*' dereference operator takes an rvalue and produces an lvalue
	int arr[] = {1,2};
	int *p = &arr[0];
	*(p + 1) = 10;						//	'p+1' is an rvalue, '*(p+1)' is an lvalue

	//	Example: '&' address-of operator takes an lvalue and produces an rvalue
	int var2 = 10;
	int* pvar2 = &var2;
	//int* bad_addr1 = &(var2+1);							//	error, cannot take address of rvalue
	//int* bad_addr2 = &move(var1);							//	error, cannot take address of rvalue


	//	Example: non-const lvalue references cannot be assigned rvalues
	const string& csref = string("hello");
	//string& sref = string("hello");				//	error, non-const lvalue-reference cannot bind to temp value


	//	Example: rvalue-references initalized from constants are not const and can be modified
	cout << "int&& rrefvar2 is non-const:\n";
	int&& rrefvar2 = 3;
	cout << "rrefvar2=(" << rrefvar2 << ")\n";
	rrefvar2 = 5;
	cout << "rrefvar2=(" << rrefvar2 << ")\n";
	cout << "\n";
	//	Ongoing: 2022-02-27T03:07:29AEDT (it bothers me that a constant value assigned to an rvalue-reference is not implicitly const)


	cout << "print_rvalue_address:\n";
	cout << "&var1=(" << &var1 << ")\n";
	print_rvalue_address(move(var1));
	print_rvalue_address(27);
	//print_rvalue_address(var1);						//	error, lvalue argument is invalid
	cout << "\n";

	assign_to_rvalue(move(var1), 27);
	assign_to_rvalue(53, 99);
	assign_to_rvalue(move(var1), a);
	//assign_to_rvalue(var1, 27);						//	error, 1st argument must be rvalue
	//assign_to_rvalue(var1, a);						//	error, 1st argument must be rvalue

	assign_urefT_i(move(var1), 99);
	assign_urefT_i(53, 99);			
	assign_urefT_i(move(var1), a);
	//assign_urefT_i(var1, 99);							//	error, conflicting deduced types 'int&' vs 'int'
	//assign_urefT_i(var1, a);							//	error, conflicting deduced types 'int&' vs 'int'

	//assign_urefT_ii(move(var1), 99);					//	error, 2nd argument must be lvalue
	//assign_urefT_ii(53, 99);							//	error, 2nd argument must be lvalue
	assign_urefT_ii(move(var1), a);
	//assign_urefT_ii(var1, 53);						//	error, conflicting deduced types 'int&' vs 'int'
	//assign_urefT_ii(var1, a);							//	error, conflicting denotes types 'int&' vs 'int'


	(void) var2; (void) c; (void) rvar1; (void) crefx2; (void) rrefvar1; (void) rrefvar2; (void) pvar2; (void) csref;
	return 0;
}

