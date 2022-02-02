#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Calls to inline functions are replaced with the body of that function, elimininating the overhead of a function call (usually) at the expense of an increase in size of the resulting binary (for very trivial functions inlining may decrease binary size compared to the equivalent function call). 
//	Inline is a request to the compiler, not a command: functions with recursion, loops, and (almost all) virtual functions will not be inlined. Typically there is a diagnostic warning by the compiler should it fail to inline a function.
//	Inlining should be used deliberately, it is not an optimization that should be applied to functions by default
//	Inlining is usually disable for debug build settings


//	Inlining can be requested implicitly or explicitly. 
//	A function whose definition is given inside a class definition is implicitly inlined. 
class Person {
public:
	int age() const { return theAge; }			//	implicit inline request
private:
	int theAge;
};
//	A function definition that is preceded with 'inline' is explicitly inlined. 
template<typename T>
inline const T& max(const T& a, const T& b) {	//	explicit inline request
	return a < b ? b : a;
}
//	if a template is declared inline, all functions instantiated from it are candidates for inlining

//	A compiler may also refuse to inline a function (that was otherwise a valid candidate for inlining) if it is used through a function pointer
inline void f() {}
void (*pf)() = f;

//	If an exception occurs during the construction of an object, any parts of the object that have been fully constructed are automatically destroyed. The code to support this is implicitly added to each constructor by the compiler. This additional hidden complexity makes constructors and destructors poor candidates for inlining. 

//	Declaring a library function inline means clients who use that function must recompile whenever it changes, whereas for a non-inline function normally it is only required that they relink.

int main()
{
	f();		//	will be inlined
	pf();		//	probably wont be inlined


	return 0;
}

//	Summary:
//		Functions can be implicitly inlined by defining them inside a class definition, or explicitly inlined with the 'inline' keyword. A call to an inline function is replaced by the body of that function in the object code. Inlining is a request to the compiler not a command, and it may be ignored.
//		Limit inlining to small, frequently called functions
//		Don't declare function templates inline just because they appear in a header file

