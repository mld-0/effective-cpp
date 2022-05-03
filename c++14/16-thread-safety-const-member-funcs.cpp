//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;
//	{{{2

//	TODO: 2022-05-03T21:18:56AEST effective-c++, item 16, thread safety const member funcs, (an example of) thread1/thread2 calling 'p.roots()' with/without mutex
//	TODO: 2022-05-03T21:32:50AEST effective-c++, item 16, thread safety const member funcs, (a book on the topic of exception/thread safety)

//	Ongoings:
//	{{{
//	Ongoing: 2022-05-03T21:38:00AEST (provide) definitions of 'thread/exception safe'
//	Ongoing: 2022-05-03T21:38:29AEST 'Polynomial2D/Polynomial2D_sync' are largely redundant, and too bulky, succinct/less-redundant example?
//	}}}

//	Example: not-threadsafe, produces a data race for 'rootsAreValid'.
//	Such reading and writing the same memory without synchronization is undefined behaviour.
class Polynomial2D {
public:
	Polynomial2D(double x1, double x2, double x3) 
		: a(x1), b(x2), c(x3)
	{}
	using Roots_t = std::vector<double>;
	Roots_t roots() const {
		if (!rootsAreValid) {
			calculate_roots();
			rootsAreValid = true;
		}
		return rootValues;
	}
private:
	void calculate_roots() const {
		double x1 = (-b + sqrt(b*b-4*a*c)) / (2*a);
		double x2 = (-b - sqrt(b*b-4*a*c)) / (2*a);
		rootValues = {x1, x2};
	}
	double a, b, c;
	mutable bool rootsAreValid{false};
	mutable Roots_t rootValues{};
};


//	Example: std::mutex
class Polynomial2D_sync {
public:
	Polynomial2D_sync(double x1, double x2, double x3) 
		: a(x1), b(x2), c(x3)
	{}
	using Roots_t = std::vector<double>;
	Roots_t roots() const {
		std::lock_guard<std::mutex> g(m);
		if (!rootsAreValid) {
			calculate_roots();
			rootsAreValid = true;
		}
		return rootValues;
	}
private:
	void calculate_roots() const {
		double x1 = (-b + sqrt(b*b-4*a*c)) / (2*a);
		double x2 = (-b - sqrt(b*b-4*a*c)) / (2*a);
		rootValues = {x1, x2};
	}
	double a, b, c;
	mutable std::mutex m;
	mutable bool rootsAreValid{false};
	mutable Roots_t rootValues{};
};


//	Example: std::atomic
//	<>

int main()
{
	return 0;
}

//	Summary:
//		const member functions <(and anything else?)> should be thread-safe unless one is *certain* they will never be used in a concurrent context
//		<(std::mutex vs std::atomic)>
//		<(any info beyond book chapter?)>

