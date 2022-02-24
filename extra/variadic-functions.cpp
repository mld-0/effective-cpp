#include <iostream>
#include <vector>
#include <string>
#include <cstdarg>
using namespace std;

//	LINK: https://en.cppreference.com/w/c/variadic
//	LINK: https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c

//	<(Varidaic functions are a pre-C++11 (C) solution, not-type-safe and error-prone. Use Varidaic templates)>
//	A Varidaic function is a function which takes a variable number of arguments. 

//	<cstdarg> defines the type 'std::va_list', and functions for accessing variadic arguments from the function body:
//		void va_start(std::va_list ap, parm_n) 				//	access to variadic func args following func arg 'param_n.
//		T va_arg(std::va_list ap, T) 						//	Access the next variadic func arg as type T.
//		void va_copy(std::va_list dest, std::va_list src)	//	<C++11> make a copy of variadic function args
//		void va_end(std::va_list ap) 						//	Cleanup 'ap' initalized by va_start/va_copy.

//	Default conversions: First lvalue-to-rvalue, array-to-pointer, and function-to-pointer conversions are made, then:
//		nullptr_t -> void*
//		float -> double
//		bool/char/short/<unscoped-enums> -> int <or wider>

//	Forms:
//int printx(const char* fmt, ...); 	// same as above (extraneous comma is allowed for C compatibility)
//int printy(..., const char* fmt); 	// error: ... cannot appear as a parameter
//int printz(...); 						// valid, but the arguments cannot be accessed portably

//	One can't count the number of variadic arguments, instead the caller must indicate this, either:
//		Pass the number of arguments as the first variable
//		Require the last variable argument to be null, zero or whatever
//		Have the first argument describe what is expected (eg. the printf format string dictates what arguments should follow)

//	<(It is a problem if there are more arguments that characters in string 'fmt')>
void my_printf(const char* fmt...)		//	'(const char* fmt, ...)' also valid
{
	char delim = '\n';
	va_list args;
	va_start(args, fmt);
	while (*fmt != '\0') {
		if (*fmt == 'i') {
			int i = va_arg(args, int);
			cout << i << delim;
		} else if (*fmt == 's') {
			char* s = va_arg(args, char*);
			cout << s << delim;
		} else if (*fmt == 'c') {
			int c = va_arg(args, int);
			cout << static_cast<char>(c) << delim;
		} else if (*fmt == 'f') {
			double f = va_arg(args, double);
			cout << f << delim;
		}
		++fmt;
	}
	va_end(args);
}


//	Ongoing: 2022-02-24T21:48:52AEDT Return the maximum of a given series of values, with the first giving the number of values (we can't say, 'while there are variadic variables remaining' (much less what type they are)?)
int maxof(int n_args, ...)
{
	va_list args;
	va_start(args, n_args);
	int max_val = va_arg(args, int);
	for (int i = 1; i < n_args; ++i) {
		int temp = va_arg(args, int);
		if (temp > max_val) { 
			max_val = temp; 
		}
	}
	va_end(args);
	return max_val;
}
//	<(Stackoverflow, on the above function: It looks bad, it's unsafe, and it's full of technical details that have nothing to do with what you're conceptually trying to achieve. Instead, consider using overloading or inheritance/polymorphism, builder pattern (as in operator<<() in streams) or default arguments etc. These are all safer: the compiler gets to know more about what you're trying to do so there are more occasions it can stop you before you blow your leg off)>



int main(int argc, char* argv[]) 
{
	my_printf("icffs", 3, 'a', 1.999, 42.5, "qwerty");		//	format string matches following arguments
	cout << "\n";

	cout << maxof(5, 1,2,7,4,9) << "\n";					//	n_args matches number of following arguments
	cout << "\n";

	cout << "done\n";
	return 0;
}

