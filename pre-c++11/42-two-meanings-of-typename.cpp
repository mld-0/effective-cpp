#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	'typename' has two uses in C++. The first is for template type parameters.
//	When used in template type parameters, 'class' and 'typename' are equivalent:
template<class T> class Widget_i {};
template<typename T> class Widget_ii {};
//	(Some developers use 'typename' when any parameter is allowed, and reserve 'class' for when a user defined type is required)

//	A name that depends of a template parameter is a 'dependent name'. When this name is nested inside a class, it is a 'nested dependent name'.
//	T::const_iterator is a 'nested dependent type name'
//	Nested dependent names can lead to parsing difficulties. Consider:
template<typename T>
void Example_i(const T& container) {
	//	Declaring a local variable 'x' as a pointer to a 'T::const_iterator'
	//	For this statement to be valid, T::const_iterator must be a type. 
	//T::const_iterator *x;			//	invalid

	//	Since there is no way for the compiler to know whether T::const_iterator is a type until T is known, we must inform the compiler that it is a type with the 'typename' keyword
	typename T::const_iterator *x;
}
//	The general rule: anytime one referes to a nested dependent type name in a template, it must be immediately preceded by keyword 'typename'

//	Example: function recieving a template parameter container type, and an iterator of that container
template<typename T>
void Example_ii(const T& container, typename T::iterator iter);

//	The exceptions to this rule: typename must not precede nested dependent type names in a base class list (list of class to be inherited from), or base class identifiers in a member initalization list.
template<typename T>
class Base { class Nested {}; };
template<typename T>
class Derived: public Base<T>::Nested {		//	base class list, typename not allowed
public:
	explicit Derived(int x) 
		: Base<T>::Nested(x)				//	base class identifer in member initalization list, typename not allowed
	{
		//	typename required, not a base class list or identifier in member initalization list
		typename Base<T>::Nested temp;		
	}
};

//	Consider: making a copy of the value of IterT of the same type 'IterT' points to
template<typename IterT>
void workWithIterator_i(IterT iter) {
	//	this is a nested dependent type name, so must be preceded by 'typename'
	typename std::iterator_traits<IterT>::value_type temp(*iter);
	//	<(is this 'old' C++?)>
}

//	typedefs are variables containing type names (since C++ allows types that are far too verbose to type repeatedly)
template<typename IterT>
void workWithIterator_ii(IterT iter) {
	typedef typename std::iterator_traits<IterT>::value_type IterT_ValueT;
	IterT_ValueT temp(*iter);
}

//	Enforcement of the rules regarding the second use of typename may vary from compiler to compiler

int main()
{
	return 0;
}
//	Summary:
//		When declaring template parameters, 'class' and 'typename' are interchangeable.
//		Nested dependent type names must be identified with 'typename', except in base class lists, or as a base class identifier in a member initalization list.

