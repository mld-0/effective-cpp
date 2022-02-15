#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
using namespace std;

//	LINK: https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp
//	LINK: https://www.cplusplus.com/reference/type_traits/

//	What is a type trait:
//		A type trait is a template struct that contains a member constant, which in turn holds the answer to the question the type trait asks or the transformation it performs.
//		Type traits allow compile time introspection in C++
//		<(replace by 'Concepts' in C++20?)>

//	For example, the <type_traits> header provides 'std::is_floating_point':
//template<typename T>
//struct is_floating_point;
//	This type trait tells whether a type T is floating point or not. The member constant, called 'value' for type traits that ask a question, is either true or false according to the template argument type.

//	Another example is 'std::remove_reference'
//template<typename T>
//struct remove_reference;
//	This type trait turns type T& into type T. The member constant, called 'type' for those type traits that modify a type, contains the result of the transformation.

//	<(type traits are implemented as template structs (which are known as trait classes))>

//	Example: we have an algorithm, with one implementation for signed values and another implementation for unsigned values
void algorithm_signed(int i) { cout << "algorithm_signed\n"; }
void algorithm_unsigned(unsigned u) { cout << "algorithm_unsigned\n"; }
//	TODO: 2022-02-13T20:13:40AEDT effective-c++, item 47, example-1, how to call algorithm_signed/algorithm_unsigned without resorting to constexpr (C++17) (using overloading, as done in (item 47)?)
template<typename T>
void algorithm(T t) {
	if constexpr(std::is_signed<T>::value) {
		algorithm_signed(t);
	} else if constexpr(std::is_unsigned<T>::value) {
		algorithm_unsigned(t);
	} else {
		static_assert(std::is_signed<T>::value || std::is_unsigned<T>::value, "Must be signed or unsigned");
	}
}


//	Example: turn a type T into an rvalue reference T&&
template<typename T>
typename remove_reference<T>::type&& move(T&& arg) {
	return static_cast<typename remove_reference<T>::type&&>(arg);
}


class Widget {};

int main()
{
	//	Type trait usage:
	cout << "std::is_floating_point<Widget>::value: " << std::is_floating_point<Widget>::value << "\n";
	cout << "std::is_floating_point<float>::value: " << std::is_floating_point<float>::value << "\n";
	cout << "std::is_floating_point<double>::value: " << std::is_floating_point<double>::value << "\n";
	cout << "std::is_floating_point<int>::value: " << std::is_floating_point<int>::value << "\n";
	cout << "\n";
	//	The compiler will generate a struct for each type the struct template is instantiated with. The attribute 'value' is static (the value is generated at compile time, and must be accessed with '::')

	//	Beautifying type traits:
	//	C++14 introduces a simplified syntax:
	cout << "std::is_signed_v<Widget>: " << std::is_signed_v<Widget> << "\n";	//	is_signed<Widget>::value
	cout << "std::is_signed_v<float>: " << std::is_signed_v<float> << "\n";		//	is_signed<float>::value
	

	return 0;
}

