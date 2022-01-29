#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	An interface is the means by which clients interact with code
//	Its design should help clients use it correctly

//	Consider what sort of mistakes a client might make
class Date_i {
public:
	Date_i(int day, int month, int year);
	//	Obvious mistakes: should be 30,3,1995
	//		Date_i d(3, 30, 1995)		
	//		Date_i d(40, 3, 1995)		
};

//	Ongoing: 2022-01-30T01:10:20AEDT C++, when to use structs vs classes?
//	One solution to this problem might be introducing new types:
//	The type system should be used as much as possible to prevent invalid code from compiling

//	<(this solution in particular is) fugly?>
struct Day {
	explicit Day(int d) : val(d) {}		// note we have disable implicit conversion for our types
	int val;
};
struct Month {
	explicit Month(int m) : val(m) {}
	int val;
};
struct Year { 
	explicit Year(int y): val(y) {}
	int val;
};
class Date_ii {
public:
	Date_ii(const Day& d, const Month& m, const Year& y);
	//	Usage:
	//		Date_ii d(30, 3, 1995);							//	invalid types
	//		Date_ii d(Month(3), Day(30), Year(1995))		//	invalid types
	//		Date_ii d(Day(30), Month(3), Year(1995))		//	valid
};

//	We may further restrict values to only those that are valid with enum-like behaviour, (enums are not especially type safe, (for example they can be used like ints) (see item 02)).
//	The use of functions to represent specific values is due to the initalization of non-local static objects being problematic (see item 04)
class Month_ii {
public:
	static Month_ii Jan() { return Month_ii(1); }
	static Month_ii Feb() { return Month_ii(2); }
	//	...
	static Month_ii Dec() { return Month_ii(12); }
private:
	explicit Month_ii(int m): val(m) {}
	int val;
};


//	Another way to prevent client errors is to restrict what can be done with a type
//	(see item 03 about const-qualifying the return value from 'operator*' prevents the error 'a * b = c')
//	in general aim for types that behave consistently with the built-in types (if in doubt, do as 'int' does<?>)


class Investment {};
//	Requiring the client to remember to do something is inviting incorrect use
Investment* createInvestment_raw();
//	eg: returning a raw pointer from a factory function requires the user to remember to call 'delete' (they may also attempt to call it more than once, or use the incorrect form)
//	For this reason, returning a smart pointer from a factory function is likely better interface design:
//	<unique_ptr vs shared_ptr, (book uses shared_ptr as unique_ptr is unavailable)>
unique_ptr<Investment> createInvestment();

//	Both shared_ptr and unique_ptr can have custom deleters specified:
//	TODO: 2022-01-30T01:29:24AEDT effective-c++, item 18 interface design basics, (dedicated 'extra/' example for) custom deleters for shared_ptr and unique_ptr

//	smartpointers also solve the 'cross-DLL problem' <(is this one of those old things that went away?)>, where an object is created using 'new' from one DLL and destroyed using 'delete' from another DLL

//	Pass 'nullptr' or 'static_cast<T*>(0)' instead of '0' to shared_ptr constructor to create a null smartpointer

//	smartpointers have performance penalties <(especially in multithreaded environments)>, but the reduction in client errors if oftern considered worth the cost

int main()
{
	return 0;
}

//	Summary:
//		strive to make interfaces easy to use correctly and hard to use incorrectly
//		include consistency in interfaces and behavioural compatibility with built-in and standard library types
//		create specialized types and allow the type system to detect usage errors
//		smartpointers eliminate certain resource management responsibilities from the client

