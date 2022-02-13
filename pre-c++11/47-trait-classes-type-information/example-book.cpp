//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
using namespace std;

//	TODO: 2022-02-13T19:07:51AEDT effective-c++, item 47 trait classes type information, book provided example, 'iterator_category' typedef and 'iterator_traits' struct (is) exceedingly unhelpful (this being a difficult subject, and excedingly clear example is called for?)
//	TODO: 2022-02-13T20:33:55AEDT effective-c++, item 47 trait classes type information, providing std::iterator_traits<> for a custom container iterator (for (at least) 'iterator_category' and 'value_type')


//	advance: moves a specified iterator a specified distance
//template<typename IterT, typename DistT>
//void advance(IterT& iter, DistT d);
//	advance does 'iter += d' for random access iterators, or by iteratively applying '++' or '--' for less powerful iterators

//	Categories of iterators:
//		input iterators, move forward one step at a time, can only read what they point to, and can only read it once
//		output iterators, move forward one step at a time, can only write what they point to, and can only write to it once
//		forward iterators, move forward one step at a time, can read/write multiple times
//		bidirectional iterators, move forwards/backwards one step at a time, can read/write multiple times
//		random access iterators, move forwards/backwards arbitrary distance, can read/write multiple times
//	Each iterator category has a 'tag struct' in the standard library:
//struct input_iterator_tag {};
//struct output_iterator_tag {};
//struct forward_iterator_tag: public input_iterator_tag {};
//struct bidirectional_iterator_tag: public forward_iterator_tag {};
//struct random_access_iterator_tag: public bidirectional_iterator_tag {};
//	The inheritance relationships among these structs are valid 'is a' relationships.

//	Ongoing: 2022-02-13T18:45:02AEDT (is this old C++?) (in linkedlist-uniqueptr, we use 'using iterator_category = std::bidirectional_iterator_tag;')

//	Consider implementing 'advance()': we want a function which can increment a random access iterator by the given offset in one operation, only resorting it iterative incrementing/decrementing for iterators which do not support random access. This requires us to know whether IterT is a random access iterator type.
//	This is the purpouse of 'traits': getting information about a type during compilation. Traits aren't a keyword or predefined construct in C++, they are a technique and convention followed by programmers.
//	We require these traits to work for built-in types like raw pointers, meaning we cannot simply nest information inside types to solve the problem, this information must be external to the type.

//	The standard technique is to put it into a template, and one or more specializations of that template. For iterators, the template in the standard library is named 'iterator_traits'. By convention, traits are always implemented as structs. These are known as 'trait classes'.
//template<typename IterT>
//struct iterator_traits;
//	For each type, IterT, a typedef named 'iterator_category' is declared in the struct 'iterator_traits<IterT>', implementing the iterator category of IterT.

//	'iterator_traits' implements this in two parts: 
//	First it imposes the requirement that any user-defined iterator type must contain a nested typedef named 'iterator_category' that identifies the appropriate tag struct. 
//	For example, a class for the random-access deque:
template<typename T>
class deque {
public:
	class iterator {
		typedef random_access_iterator_tag iterator_category;
	};
};
//	A class for the bidirectional list:
template<typename T>
class list {
public:
	class iterator {
		typedef bidirectional_iterator_tag iterator_category;
	};
};
//	Ongoing: 2022-02-13T19:24:07AEDT is 'struct iterator_traits' something we are expected to declare?
//	Second, (trait class) iterator_traits just parrots back the iterator class's nested typedef:
//template<typename IterT>
//struct iterator_traits {
//	typedef typename IterT::iterator_category iterator_category;
//};

//	This works for user defined types, but not for raw pointers, because there is no such thing as a pointer with a nested typedef. (The second part of the iterator_traits implementation handles iterators that are pointers).
//	To support such iterators, iterator_traits offers a partial template specialization for pointer types (pointers act as random access iterators).
//template<typename T>
//struct iterator_traits<T*> {
//	typedef random_access_iterator_tag iterator_category;
//};

//	Designing a trait class:
//		identify some information about types to make available (i.e: the category of an iterator)
//		chose a name to identify that information (i.e: 'iterator_category')
//		Provide a tempalte and a set of specializations (e.g: 'iterator_traits') that contain the information for the types to be supported

//	Ongoing: 2022-02-13T19:12:11AEDT overloading specifying only a type (not a variable to be of that type)?
//	C++ provides a conditional construct for types that is evaluated during compilation: overloading.
//	<(Bringing us to an implementation for 'advance()')>
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
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag) {
	if (d < 0) { throw std::out_of_range("Negative distance"); }
	while (d--) ++iter;
}
//	because forward_iterator_tag inherits from input_iterator_tag, that version of doAdvance will also handle forward iterators.
//	Ongoing: 2022-02-13T19:15:36AEDT 'iterator_category()' is now a function?
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
	doAdvance(iter, d, typename std::iterator_traits<IterT>::iterator_category());
}
//	How to use a trait class:
//		Create a set of overloaded 'worker' functions or function templates that differ in a traits parameter. Implemented each function in accord with the traits information passed.
//		Create a 'master' function or function template (e.g: 'advance()') that calls the worker, passing information provided by the traits class.

//	Other traits used in the standard library include: 'iterator_traits', 'char_traits', 'numeric_limits'


int main()
{
	//cout << std::iterator_traits<deque<int>::iterator>::iterator_category << "\n";
	return 0;
}

