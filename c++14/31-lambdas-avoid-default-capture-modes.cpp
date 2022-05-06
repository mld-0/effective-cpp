//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
using namespace std;
//	{{{2
//	get_vector_randoms():
//	{{{
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
//	}}}
//	Ongoings:
//	{{{
//	Ongoing: 2022-05-07T00:38:49AEST if re-ordering the declarations of Widget member variables min/max and divisor can leave divisor undefined, (does that not imply) there is no best practice for defining member variables (in terms of each other)?
//	Ongoing: 2022-05-07T00:11:13AEST 'random_num', better as a lambda/function (don't tell me 'better as a macro'?)
//	Ongoing: 2022-05-06T23:44:03AEST best practice -> using global/static variables in lambdas?
//	Ongoing: 2022-05-06T23:42:53AEST f_ii is even worse(?) (elaborate)
//	Ongoing: 2022-05-06T23:41:26AEST 'random_num' (does '-> int' make a difference?) 
//	Ongoing: 2022-05-03T21:53:00AEST is 'avoid default capture modes' applicable to function pointers (anything other than lambdas?) -> (if only relavent to lambdas, add 'lambda' to filename (before commiting)) [...] -> (having added it, (is the question still <valid/meaningful?>))
//	}}}
//	TODO: 2022-05-07T00:36:24AEST Examples use of 'AddDivisorFilter' / 'Widget::addFilter' (featuring) undefined behaviour

//	<C++11> has two default capture modes
//			[=]		by-value
//			[&]		by-reference

//	Book contention: both these are bad.
//	Default by-reference capture can lead to dangling references, as does default by-value capture (with the added danger of the false sense of security of by-value, and making one think one's closures are self contained when they may not be). Explicitly listing the values one is capturing, (whether by value or reference), makes mistakes due to dangling variables less likely.

//	Example: random number lambda generator
auto random_num  = [](int min, int max) -> int { 	
	//	values generated are in range [min, max]
	std::srand(unsigned(std::time(nullptr))); 
	return std::rand() % (max+1) - min; 
};

//	Example: dangling references using default by-reference capture
//	'roll' is a local variable captured by reference, when function returns, roll will dangle
using FilterContainer = vector<std::function<bool(int)>>;
void AddDivisorFilter(FilterContainer& filters) {
	srand(time(0));
	int min = 1, max = 10;
	int roll = min + rand() % (max - min + 1);
	//	Ongoing: 2022-05-06T23:11:37AEST (how?) do dangling references cause things to go boom?
	auto f = [&](int x) { return x % roll == 0; };
	filters.emplace_back(f);
}
//	'roll' goes out of scope, new lambda in 'filters' now has a dangling reference


//	Example: while a lambda using default by-reference capture can be used safely in some contexts, (this is still bad practice), as this makes it possible to later move it to another context where the same by-reference captures are no longer safe
//	Ongoing: 2022-05-06T23:48:12AEST example with 'sum' (or something more likely to give a result that isn't 'false') instead of 'all_of' [...] -> sum of divisions/modulus?
template<typename C>
bool WorkWithContainer(C& container) {
	using std::begin;			//	for genericity (see item 13)
	using std::end;
	int min = 1, max = 10;
	int roll = random_num(min, max);
	//	Use of 'f' is safe in this context, because we never use 'f' outside the scope it is declared in <(how does that work as a general rule?)>
	auto f = [&](int x) { return x % roll == 0; };
	//auto f_ii = [&](int x) { return x % random_num(min, max); };
	//auto f_iii = [&min, &max](int x) { return x % random_num(min, max) == 0; };
	//auto f_iv = [&roll](int x) { return x % roll == 0; };
	if (std::all_of(begin(container), end(container), f)) {
		return true;
	} else {
		return false;
	}
}


//	Example: a pointer captured by-value may be deleted before it is used
//	<>


//	Ongoing: 2022-05-07T00:12:19AEST implicit use of 'this' pointer (this capture) -> by-value vs by-reference [...] difference between capturing 'this' by-value/reference
//	Example: implicit use of 'this' pointer
//	<(Capture (by-value, or both) only applies to non-static local variables (including parameters) visible in the scope where the lambda is created (that is, not class member variables))>
//	Every non-static member function has an implicit 'this' pointer.
class Widget {
	//	note: if 'min/max' were declared after 'divisor', they would be undefined when used to initalize it
	int min = 1, max = 10;
	int divisor;
public:
	Widget() : divisor(random_num(min, max)) {
		cout << "divisor=(" << divisor << ")\n";
	}
	void addFilter(FilterContainer& filters) const {
		auto f = [&](int x) { return x % divisor == 0; };
		//auto f_ii = [](int x) { return x % divisor == 0; };	//	error, cannot access 'divisor' without 'this' capture
		//auto f_iii = [=](int x) { return x % divisor == 0; };			//	valid
		//auto f_iv = [divisor](int x) { return x % divisor == 0; };	//	no local variable 'divisor' to be captured
		//auto f_v = [divisor = this->divisor](int x) { return x % divisor == 0; };	//	capture member variable by-value
		//auto f_vi = [divisor = divisor](int x) { return x % divisor == 0; };		//	also valid <C++14>
		filters.emplace_back(f);
	}
};
//	If instance of 'Widget' used to add new lambdas to 'filters' goes out of scope, any variables accessed implicitly through 'this' pointer are left dangling.


//	Example: by-value vs by-reference capture of 'this'
//	<>


//	Example: default by-value capture can imply static variables are being captured by-value when they are not
//	<>


int main()
{
	vector<int> r1 = get_vector_randoms(1);
	auto result1 = WorkWithContainer(r1);
	cout << "result1=(" << result1 << ")\n";
	cout << "\n";

	Widget w1;
	cout << "\n";

	return 0;
}

//	Summary:
//		Default by-reference capture can lead to dangling references
//		Default by-value capture is susceptible to dangling pointers (especially 'this'), and it can misleadingly suggest lambdas are self contained, or capture variables by-value they do not (static variables)
//		Although default capture can be used safely in some contexts, best practice is to avoid it due to the danger of later moving a lambda to a context where it is no longer safe

