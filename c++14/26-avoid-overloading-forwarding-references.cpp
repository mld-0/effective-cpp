#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

multiset<string> names;

//	Forwarding references imply type deduction - template functions or lambdas with auto parameters. <(the lesson of item 26 is do not overload template functions? - which is item *what*?)>
//	In C++, for template function with overloads, if the type of the arguments is not an exact match for the overload (even if they are convertable, i.e: passing a short to a function expecting an int), the template function is prefered. 
//	Only where the arguments are an exact match is the overload prefered over the template function.

template<typename T>
void logName(T&& name) {
	cout << "logName(T&&)=(" << name << ")\n";
	auto now = chrono::system_clock::now();
	names.emplace(forward<T>(name));
	(void) now;
}
void logName(int id) {
	cout << "logName(int)=(" << id << ")\n";
	string name = to_string(id);
	names.emplace(name);
}


class Person {
public:
	//	Manually declare default copy/move ctors
	Person(const Person& rhs) : name(rhs.name) {
		cout << "Person(const Person&)\n";
	}
	Person(Person&& rhs) : name(move(rhs.name)) {
		cout << "Person(Person&&)\n";
	};
	template<typename T>
	explicit Person(T&& n) : name(forward<T>(n)) {
		cout << "Person(T&&)\n";
	}
	explicit Person(int id) : name(to_string(id)) {
		cout << "Person(int)\n";
	}
	string name;
};


//	Example: ctors with forwarding reference become even more problematic during inheritance
//class SpecialPerson : public Person {
//public:
//	SpecialPerson(const SpecialPerson& rhs) 
//		: Person(rhs) 								//	error, calls Person(T&&), not Person(const Person&)
//	{}
//	SpecialPerson(SpecialPerson&& rhs) 	
//		: Person(move(rhs)) 						//	error, calls Person(T&&), not Person(Person&&)
//	{}
//};


int main()
{
	//	Example: Overloading a template function can lead to unexpected overload resolution results 
	string name1 = "Larry";
	int id1 = 27;
	short id2 = 53;
	logName(name1);
	logName(id1);
	//logName(id2);					//	error, calls logName(T&&), not logName(int)
	cout << "\n";

	//	Example: Overloading a template ctor can lead to unexpected overload resolution results
	Person p1("Nancy");				//	calls Person(T&&)
	const Person p2("Jeff");		//	calls Person(T&&)
	//Person cloneOfP1(p1);			//	error, calls Person(T&&), not Person(const Person&)
	Person cloneOfP2(p2);			//	calls Person(const Person&)
	cout << "\n";

	(void) id2;
	return 0;
}

//	Summary:
//		Function that accept forwarding references are template functions.
//		Overloading on forwarding references invariably leads to unexpected overload resolution results.
//		Constructors accepting forwarding references are especially problematic, because they're typically a better match than the copy-ctor for non-const lvalues, and they can hijack Derived class calls to Base class copy and move ctors.

