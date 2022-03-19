//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
using namespace std;
//	{{{2
//	Ongoing: 2022-03-19T19:58:11AEDT book uses 'list<T,MyAlloc<T>>' (not 'unordered_map<T,MyValue<T>>') -> writing a custom allocator is too extensive an exercise for the purposes of this example(?)
template<typename T> class MyValue {};

//	TODO: 2022-03-19T20:54:37AEDT effective-c++, item 09, prefer-alias-to-typedef, <explain/outline> 'using' declarations <of/and> alias-es

//	Ongoing: 2022-03-19T19:33:02AEDT (what happens where) an alias and a typedef of the same name are declared(?) (we do not get an error when we declare typedef/alias function of the same name, but declaring 'T1' as int using both is an error(?)

//	<C++98> provides typedefs to save repeated typing of verbose types
typedef unique_ptr<unordered_map<string, string>> UPtrMap_TD;
//	<C++11> offers alias declarations for the same purpouse
using UPtrMap_AD = unique_ptr<unordered_map<string, string>>;


//	Ongoing: 2022-03-19T20:38:18AEDT use function pointer alias/typedef 'FP_TD'/'FP_AD' for <something>(?)
//	Example: alias declarations vs typedefs for function pointer types
typedef void (*FP_TD)(int, const string&);
using FP_AD = void(*)(int, const string&);
void f_i(FP_TD callback);
void f_ii(FP_AD callback);


//	Example: alias declarations can be templatized, while typedefs cannot.
template<typename T>
using MyValueList_AD = unordered_map<T,MyValue<T>>;
//	<old-C++> equivalent is to declare a typedef in a template struct
template<typename T>
struct MyValueList_TD {
	typedef unordered_map<T, MyValue<T>> type;
};
//	'MyValueList_TD' is considered a dependent type for the purpouses of requiring the use of 'typename' to declare variables with it. 'MyValueList_AD' is not, because it is an alias template, and therefore must be a type.


//	Example: type traits, an assortment of templates which perform type transformations
template<typename T>
void type_transformations() {
	//	<C++11>:
	typename std::remove_const<T>::type x1;				//	Yields T from const T
	typename std::remove_reference<T>::type x3;			//	Yields T from T&/T&&
	typename std::add_lvalue_reference<T>::type x5;		//	T -> T&
	//	<C++14>:
	std::remove_const_t<T> x2;
	std::remove_reference<T> x4;
	std::add_lvalue_reference_t<T> x6;
}


//	Example: implement 'my_remove_const_t' in terms of 'remove_const'
template<typename T>
using my_remove_const_t = typename remove_const<T>::type;


//	Ongoing: 2022-03-19T20:45:04AEDT Need to include array size to pass array be reference?
//	Example: use array references to prevent array parameter decaying to pointer <(this requires the length of the array be included with the parameter?)>
template<int L>
void pass_array(int (&arr)[L]) {
	cout << sizeof(arr) / sizeof(*arr) << "\n";
}


int main()
{
	//	Use of template alias/typename, note the later requires the use of 'typename'
	MyValueList_AD<int> l1;
	typename MyValueList_TD<int>::type l2;

	int values[20];
	pass_array(values);
	cout << "\n";

	return 0;
}

//	Summary:
//		Alias (using) declarations perform the same function as typedefs, but they support templatization. Typedef template types are declared as a member of a wrapper template struct.
//		C++11 type transformations: 'std::transformation<T>::type', C++14 versions: 'std::transformation_t<T>'.

