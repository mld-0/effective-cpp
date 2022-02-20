#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;

//	auto variables have their type deduced from their initalizer (so by definition they must be initalized)
int x1;				//	(potentially) uninitalized
//auto x2;			//	error, initalizer required
auto x3 = 0;		//	x3's value is well-defined


//	Example: auto variable for template iterator value
template<typename It>
void dwim(It b, It e) {
	//	alternatively: 'typename std::iterator_traits<It>::value_type currValue = *b;'
	auto currValue = *b;
}

//	Example: lambda with auto parameters and return type
auto derefLess = [](const auto& p1, const auto& p2) { return *p1 < *p2; };
//	Using a template instead of auto parameters
template<typename T> 
auto derefUPLess = [](const unique_ptr<T>& p1, const unique_ptr<T>& p2) { return *p1 < *p2; };

//	Example: lambda-as-std::function with verbose duplication of type parameters
std::function<bool(const std::unique_ptr<int>&, const std::unique_ptr<int>&)> derefUPLessInt_i = [](const std::unique_ptr<int>& p1, const std::unique_ptr<int>& p2) { return *p1 < *p2; };
//	std::function is a template that generalizes the idea of a function pointer. Using 'auto' is almost certain to be faster and use less memory than std::function.
//	Ongoing: 2022-02-20T21:25:26AEDT why no unused variable warning is generated <for/about> derefUpLessInt_i? (because it's an std::function?)

//	<(using decltype(auto) uses decltype rules to determine type (see item 03))>

int main()
{
	(void) derefLess;

	//	TODO: 2022-02-20T21:52:52AEDT effective-c++, item 05, prefer-auto-declarations, declaring as auto each <kind/type> of C++ number
	auto int1 = 3;
	auto long1 = 3l;

	//	A common mistake: 'unsigned' is a 32 bit unsigned integer, whereas 'v1.size()' is a std::vector<int>::size_type, an unsigned value that is implementation dependent
	vector<int> v1 = { 53, 9, 7, 4, 12, 8, 5 };
	unsigned size1_i = v1.size();
	//	using 'auto' ensures we are using a variable of the correct type/size
	auto size1_ii = v1.size();
	size1_i = size1_ii = 0;

	//	Example: iterating through an unordered_map
	std::unordered_map<string,int> m1 = { {"abc", 3}, {"def", 4}, {"hij", 5} };
	for (const std::pair<const string,int>& p: m1) {		//	note: map key must be const
		cout << "p=(" << p.first << ", " << p.second << ") ";
		cout << &p << "\n";
	}
	for (const auto& p: m1) {								//	auto eliminates chances for such type errors
		cout << "p=(" << p.first << ", " << p.second << ") ";
		cout << &p << "\n";
	}
	//	Using the wrong type results in slower iteration (new objects being created as-opposed to references into map)
	for (const std::pair<string,int>& p: m1) {	
		cout << "p=(" << p.first << ", " << p.second << ") "; 
		cout << &p << "\n";
	}
	cout << "\n";

	//	TODO: 2022-02-20T21:47:10AEDT effective-c++, item 05, prefer-auto-declrations, vim (and YCM) and how to get the type of an 'auto' variable?
	//	auto can result in types that are neither anticipated nor desired (see item 02 and item 06). It is an option, not a mandate. Choice of variable names and C++ conventions can suggest what a variable is. 
	//	Book claim: there is nothing contradictory about automatic types and the creation and maintinance of industrial-strength code bases.
	//	Use of auto eliminates opportunities for type errors and minimizes the need for refactoring when updating types.

	return 0;
}

//	Summary:
//		'auto' variables must be initalized, prevent errors due to mismatching types, ease the process of refactoring, and require less typing than explicitly specified types.
//		They are subject to pitfalls (see item 02, item 03, and item 06)

