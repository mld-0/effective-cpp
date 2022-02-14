//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <limits>
#include <list>
using namespace std;
//	{{{2

//	Ongoing: 2022-02-14T18:40:07AEDT case of reverse iterators and our custom advance() function?

//	A type trait is a small object whose purpouse is to carry information used by another object or algorithm to determine 'policy' or 'implementation details'. <(These are defined in template structs, and are called trait classes. They contain member constants, which hold the answer(s) to the question the type trait asks, or the transformation it performs)>.
//	LINK: https://www.cplusplus.com/reference/type_traits/

//	Example: the <type_traits> header provides 'std::is_floating_point':
//	This type trait tells whether a type T is floating point or not. The member constant, called 'value' for type traits that ask a question, is either true or false according to the template argument type.


//	Example: 'advance()', using 'iterator_traits'
//	C++ provides a conditional constructs for types that is evaluated during compilation: overloading. We overload a function for each iterator type, selecting which to call with the iterator type as <(an unnamed argument)>
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag) {
	iter += d;
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag) {
	if (d >= 0) { while (d--) ++iter; }
	else { while (d++) --iter; }
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag) {		//	also handle forward_iterator_tag
	if (d < 0) { throw std::out_of_range("Negative distance"); }
	while (d--) ++iter;
}
template<typename IterT, typename DistT>
void advance_i(IterT& iter, DistT d) {
	doAdvance(iter, d, typename std::iterator_traits<IterT>::iterator_category());
}
//	Alternative flawed implementation for 'advance()' using typeid comparison: even though '+=' is only called if the iterator has been determined to be random access, the template function will not compile if insubstantiated with an iterator type which does not support random access.
template<typename IterT, typename DistT>
void advance_ii(IterT& iter, DistT d) {
	if (typeid(typename std::iterator_traits<IterT>::iterator_category) == typeid(std::random_access_iterator_tag)) {
		iter += d;
	} else {
		if (d >= 0) { while (d--) ++iter; }
		else { while (d++) --iter; }
	}
}
//	This template is invalid for any 'IterT' that is not a random-access-iterator, since even though the 'iter += d' statement will not be executed unless we have determined our iterator is random access, it still must be valid for all types with which the function is instantiated in order to be compiled.


//	Example: move(), using 'remove_reference<T>'
template<typename T>
typename remove_reference<T>::type&& move(T&& arg) {
	return static_cast<typename remove_reference<T>::type&&>(arg);
}


//	Type traits for Custom Iterators:
//	<old-C++> declare typedefs without inheriting from std::iterator:
template<long FROM, long TO>
class Range_i {
public:
	class iterator {
        long num = FROM;
    public:
		//	iterator traits (must define all 5 for iterator_traits<> to work)
		typedef long difference_type;
		typedef long value_type;
		typedef const long* pointer;
		typedef const long& reference;
		typedef std::forward_iterator_tag iterator_category;

        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return num;}
    };
    iterator begin() {return FROM;}
    iterator end() {return TO >= FROM? TO+1 : TO-1;}
};
//	<old-C++> inherit from std::iterator:
template<long FROM, long TO>
class Range_ii {
public:
    // member typedefs provided through inheriting from std::iterator
    class iterator: public std::iterator<
                        std::forward_iterator_tag, // iterator_category
                        long,                      // value_type
                        long,                      // difference_type
                        const long*,               // pointer
                        const long&                // reference
                                      > {
        long num = FROM;
    public:
        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return num;}
    };
    iterator begin() {return FROM;}
    iterator end() {return TO >= FROM? TO+1 : TO-1;}
};
//	<new-C++> 'using' declarations:
template<long FROM, long TO>
class Range_iii {
public:
    class iterator {
        long num = FROM;
    public:
		//	iterator traits (must define all 5 for iterator_traits<> to work)
        using difference_type = long;
        using value_type = long;
        using pointer = const long*;
        using reference = const long&;
        using iterator_category = std::forward_iterator_tag;
		//	functions
        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return num;}
    };
    iterator begin() {return FROM;}
    iterator end() {return TO >= FROM? TO+1 : TO-1;}
};
//	<any-way-newer-than-using-declarations?>

//	Ongoing: 2022-02-14T18:57:51AEDT (any) further examples?


//	type traits only available through compiler magic:
//	LINK: https://github.com/Quuxplusone/from-scratch/blob/master/include/scratch/bits/type-traits/compiler-magic.md

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
	cout << "\n";

	//	numeric_limits:
	cout << "std::numeric_limits<long>::max(): " << std::numeric_limits<long>::max() << "\n";
	cout << "std::numeric_limits<long>::min(): " << std::numeric_limits<long>::min() << "\n";
	//	'is_specified' tells whether numeric_limits information is available for given type
	cout << "std::numeric_limits<long>::is_specified: " << std::numeric_limits<long>::is_specialized << "\n";
	cout << "std::numeric_limits<string>::is_specified: " << std::numeric_limits<string>::is_specialized << "\n";
	cout << "\n";

	vector<int> v1 = { 2, 3, 5, 8, 9, 4 };
	list<int> v2 = { 2, 3, 5, 8, 9, 4 };
	auto iter1 = v1.begin();
	auto iter2 = v2.begin();
	advance_i(iter1, 3);
	advance_i(iter2, 3);			//	advance_i supports all iterator types
	cout << *iter1 << "\n";
	cout << *iter2 << "\n";
	cout << "\n";

	vector<int> v3 = { 2, 3, 5, 8, 9, 4 };
	list<int> v4 = { 2, 3, 5, 8, 9, 4 };
	auto iter3 = v3.begin();
	auto iter4 = v2.begin();
	advance_ii(iter3, 3);
	//advance_ii(iter4, 3);			//	cannot call advance_ii with non-random-access iterator (must support '+=')
	cout << *iter3 << "\n";
	cout << *iter4 << "\n";
	cout << "\n";

	std::iterator_traits<Range_i<0,5>::iterator>::value_type x1 = 5;
	std::iterator_traits<Range_i<0,5>::iterator>::iterator_category c1;

	std::iterator_traits<Range_ii<0,5>::iterator>::value_type x2 = 5;
	std::iterator_traits<Range_ii<0,5>::iterator>::iterator_category c2;

	std::iterator_traits<Range_iii<0,5>::iterator>::value_type x3 = 5;
	std::iterator_traits<Range_iii<0,5>::iterator>::iterator_category c3;

	return 0;
}

//	Ongoing: 2022-02-14T19:03:08AEDT <(use <of/for> constexpr? in making compile-time type decisions)> 

//	Summary:
//		Trait classes make information about types available during compilation. They're implemented using template structs and template specialization.
//		In conjunction with overloading ((example given is) with iterator category as unnamed argument), trait classes make it possible to perform compile-time if-else tests on types. 

