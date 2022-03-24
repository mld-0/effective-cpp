#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Ongoing: 2022-03-24T19:59:38AEDT cannot cast 'int*' to 'char*'
//	Ongoing: 2022-03-24T20:20:56AEDT meaning of a 'const volatile' (we cannot change this value, but someone else can?)

//	In <C++98>, to prevent client use of implicitly generated class member functions, (for example, copy ctor/assignment) they are declared private and not given a definition. It is not possible to use this method to disable specific instantiations of a member template function.
class Widget_i {
private:
	Widget_i(const Widget_i& lhs);						//	no definition
	Widget_i& operator=(const Widget_i& lhs);			//	no definition
};

//	<C++11> provides '= delete', to explicitly mark a function as not available.
//	By convention, deleted functions are declared public, not private, as it generally results in better error messages.
class Widget_ii {
public:
	Widget_ii(const Widget_ii& lhs) = delete;
	Widget_ii& operator=(const Widget_ii& lhs) = delete;
};

//	'= delete' is a superior method to disable functions than declaring them private and unimplemented. If a member or friend function (which have access to private functions) attempts to use a private function with no defintion, the error is not detected until link time. Attempting to use a deleted function produces an error at compile time.


//	In C++, <most?> primative types implicitly convert to int, resulting in calls which might not make sense. We can prevent such calls by overloading the function in question and deleting those we wish to reject:
bool isLucky(int num) { return true; }
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;							//	rejects doubles and floats
//	Even though deleted functions cannot be used, they are considered during overload resolution.

//	We can use delete to disable instantiations of a template (that is, to specify types that cannot be used).
template<typename T> void processPointer(T* ptr) { cout << "processPointer(" << ptr << ")\n"; }
template<> void processPointer<void>(void*) = delete;
template<> void processPointer<char>(char*) = delete;
//	If void* and char* are invalid, it is probably necessary to disable their const equivalents as well:
template<> void processPointer<const void>(const void*) = delete;
template<> void processPointer<const char>(const char*) = delete;
//	To be through, it is neccessary to also delete 'volatile'
template<> void processPointer<volatile void>(volatile void*) = delete;
template<> void processPointer<volatile char>(volatile char*) = delete;
//	<(combinations of const/volatile? - combinations get large quickly, there is not a better alternative? const-volatile/volatile/const are equivalent?)>
template<> void processPointer<const volatile void>(const volatile void*) = delete;
template<> void processPointer<const volatile char>(const volatile char*) = delete;
//	<(wchar_t, char16_t, char32_t, ... (ugh))>


int main()
{
	//	Use of 'isLucky':
	isLucky(3);
	//isLucky('z');										//	error, deleted function
	//isLucky(3.5);										//	error, deleted function

	//	Use of 'processPointer':
	int i1 = 7;
	char c1 = 'a';
	processPointer(&i1);
	//processPointer(static_cast<void*>(&i1));			//	error, deleted function
	//processPointer(&c1);								//	error, deleted function

	return 0;
}

//	Summary:
//		Prefer '= delete' to the use of private undefined functions.
//		Any function may be deleted, including non-member functions, and template instantiations.

