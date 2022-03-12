#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;
//	See (item 26) for (some of) the problems with overloading template functions with forwarding reference parameters.
//	Alternatives:

//	Abandon overloading: Use a different name for would-be overloads.

//	Pass by const T&: Revert to older C++, and simply forgo use of forwarding references, and the efficiency they offer.

//	Pass by value: (see item 41), pass objects by value whenever they are to be copied <(copy-epsilon comes in how?)>.

//	Tag dispatch: use an additional parameter whose purpose is to determine which overload should be selected.
//	Overload resolution is performed at compile time. 
multiset<string> names;
template<typename T>
void logName(T&& name) {
	logNameImpl(std::forward<T>(name), std::is_integral<typename std::remove_reference<T>::type>());
}
template<typename T>
void logNameImpl(T&& name, std::false_type) {
	names.emplace(std::forward<T>(name));
}
void logNameImpl(int id, std::true_type) {
	string name = to_string(id);
	names.emplace(name);
}

//	Constrain templates that take forwarding references: 'enable_if'
//	A template using 'enable_if' is enabled only if the specified condition is satisfied.
//	(item 26 explains that) a ctor that takes a forwarding reference will be preferred to the copy-ctor for non-const arguments (something especially problematic for Derived classes that call their Base class ctor). (Use 'enable_if' To solve situations where an overloaded function taking a forwarding reference is too greedy, but still not greedy enough to act as a single dispatch function).

//	Example: Disable forwarding reference ctor if T either is or is derived from 'Person', or if T is integral.
//	To handle the case where T is a reference, we need 'std::decay_t<T>::type'. To compare types, we could use 'std::is_same<A,B>::value', but this does not handle the problem of Derived classes, for this we need 'std::is_base_of<A,B>::value' (for the purpose of which, a class is considered to be a Base of itself). Here we used 'enable_if_t<>' in place of 'enable_if<>::value' and 'decay_t<T>' in place of 'decay<T>::value'.
class Person {
public:
	Person(const Person& rhs) : name(rhs.name) {
		cout << "Person(const Person&)\n";
	}
	Person(Person&& rhs) : name(move(rhs.name)) {
		cout << "Person(Person&&)\n";
	}
	//	Ongoing: 2022-03-12T23:14:03AEDT why 'remove_reference_t' and not 'decay_t' for 'is_integral' case -> (its use is not necessary, or problematic somehow?)
	template<typename T, typename = typename std::enable_if_t<
			!std::is_base_of<Person, typename std::decay_t<T>>::value
			&&
			!std::is_integral<std::remove_reference_t<T>>::value
		>
	>
	explicit Person(T&& n) : name(std::forward<T>(n)) {
		cout << "Person(T&&)\n";
		//	Perfect forwarding leads to very verbose error messages when the wrong type is used. We can use a static_assert to produce a more helpful error message for incorrect types.
		static_assert( std::is_constructible<std::string, T>::value, "n must be able to construct a string" );
	}
	explicit Person(int id) : name(to_string(id)) {
		cout << "Person(int)\n";
	}
	string name;
};
//	TODO: 2022-03-12T23:06:07AEDT effective-c++, item 27, alternatives-to-overloading-forwarding-references, provide extended/brief definitions/descriptions of: enable_if/enable_if_t, decay/decay_t, is_same/<(is_same_t)>, is_base_of
//	enable_if:
//	decay:
//	is_same:
//	is_base_of:
//	is_constructible:


//	Ongoing: 2022-03-12T23:39:02AEDT the f--- (type) is the literal '3s' - something time related?
int main()
{
	Person p1("Geoff");							//	calls Person(T&&)
	Person p2(static_cast<short>(27));			//	calls Person(int)
	Person p3(p1);								//	calls Person(const Person&)
	Person p4(move(p1));						//	calls Person(Person&&)
	cout << "\n";

	return 0;
}

//	Summary:
//		Alternatives to overloading on forwarding references are: 1) use of distinct function names, 2) passing parameters by lvalue-reference-to-const or by value, and 3) using tag dispatch.
//		Containing templates with 'enable_if' allows template functions to be disabled for certain conditions/types.
//		Forwarding references have efficiency advantages, but where performance is not critical their disadvantages may make them undesirable.

