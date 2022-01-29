#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	The C++ <natrual approach> is declaring class 'WebBrowser' (along with convinence functions like 'clearBrowser()')
//	<(namespaces and multiple files)>
namespace WebBrowserStuff {

class WebBrowser {
public:
	void clearCache() {}
	void clearHistory() {}
	void removeCookies() {}

	//	This is functionality that can be provided by a non-friend non-member function that makes the same calls
	void clearEverything() {
		clearCache();
		clearHistory();
		removeCookies();
	}
};

//	OOP principles dicatate that data should be as encapsulated as possible - the member function 'clearEverything()' is less encapsulated that the non-member function 'clearBrowser()'
//	The more functions can access a piece of data, the less encapsulated that data. That is:
//	Given a choice between a member function and a non-member non-friend which provide the same functionality, the non-member function provides greater encapsulation since it doesn't (unnecessarily) increase the number of functions that can access the private parts of the class 
//	(note that this still applies where the function is a member of a different class, i.e: a utility class, being a member of a different non-friend class it still provides the greater encapsulation that the non-friend non-member function did)
void clearBrowser(WebBrowser& wb) {
	wb.clearCache();
	wb.clearHistory();
	wb.removeCookies();
}

//	Declaring the class and convinence functions seperately means the client can use the class without having to compile every convinence function, instead if the convinence functions are seperated into different sources (sharing the same namespace), only those the client needs have to be included and compiled
//	It also allows clients to extend the <library/namespace> by declaring yet more convinence functions themselves (whereas class definitions are closed to extension)

}

int main()
{
	return 0;
}

//	Summary:
//		Encapsulation of a class is increased when less code has access to the private members of that class, therefore:
//		Prefer non-member non-friend functions to member functions (where the same functionality can be achieved, i.e: where access to class private members is not requried) (these can be called convinence functions)
//		Declare these in the same namespace, in a seperate source, is a practice that increases both packaging flexibility and functional extensibility

