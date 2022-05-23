//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;
//	{{{2
//	TODO: 2022-05-23T16:35:51AEST effective-c++, warnings-sanity-check, a more reasonable set of maximalist warnings


//	Before questioning one's sanity while debugging, try enabling all warnings:
//	Note: likely produces so many warnings the bug-related-warning may be buried
//			-Weverything 


//	A more reasonable set of maximalist warnings: INCOMPLETE
//			-Wall -Wextra -Wconversion <?>


//	Enable the 'default' g++ warnings. Note: this is not even close to 'all' warnings.
//			-Wall


//	Useful warnings warnings:
//			-Wconversion			warn about implicit conversions / (possible loss of precision)



//	Example: bench-write-read-text-files, 'major-trust-issues' kind of bug
//	Problem is easy to miss without: -Wconversions
//	Especially since without 'print_full_precision_double()', these kinds of rounding errors may not be obvious/apparent.
void recieve_as_long(long x) {
	cout << "receive_as_long=(" << x << ")";
}
void print_full_precision_double(double x) {
	int precision = std::numeric_limits<double>::max_digits10;
	cout << std::setprecision(precision) << x;
}
void example_pass_dubiously_rounded_double() {
	double a = 1.0 + 0.00000000001;
	cout << "pass_dubiously_rounded_double=(";
	print_full_precision_double(a);
	cout << "), ";
	cout << "\n";
}


int main()
{
	example_pass_dubiously_rounded_double();
	return 0;
}


