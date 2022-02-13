#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;

//	LINK: https://en.cppreference.com/w/cpp/iterator/iterator
//	LINK: https://stackoverflow.com/questions/37031805/preparation-for-stditerator-being-deprecated/38103394

//	LINK: https://en.cppreference.com/w/cpp/iterator/iterator_traits
//	std::iterator_traits
//	the type trait class that provides uniform interface to the properties of LegacyIterator types. This makes it possible to implement algorithms only in terms of iterators. <(The template can be specialized for user-defined iterators so that the information about the iterator can be retrieved even if the type does not provide the usual typedefs.)>

//	In C++<version>, inheritance from std::iterator is being deprecated, (specifying properties of an iterator in C++ has an old and a new way (only two ways?) (compatibility between old/new?))

//	From linkedlist-uniqueptr, valid expressions:
//std::iterator_traits<LinkedList<int>::iterator>::value_type x;
//std::iterator_traits<LinkedList<int>::iterator>::iterator_category y;
//	LinkedList<T>::iterator (nothing more than a) being a simple (regular) nested class of (regular) LinkedList class template, (no inheritance from std::iterator, only 'using' declarations, (see below))
//using iterator_category = std::bidirectional_iterator_tag;
//using value_type = T;
//using difference_type = std::ptrdiff_t;
//using pointer = T*;
//using reference = T&;
//	this is <C++-version?> (the point being it works with 'std::iterator_traits' (a type trait class that provides uniform interface to the properties of LegacyIterator types) as is) <(different from the C++20 technique?)>



//	Ongoing: 2022-02-14T00:47:11AEDT template type for our custom iterator (the range FROM/TO do not add to the <example/demonstration/discussion>?)
//	Example: <old-C++> how to implement an input iterator by inheriting from std::iterator, from:
//	LINK: https://en.cppreference.com/w/cpp/iterator/iterator
template<long FROM, long TO>
class Range {
public:
//	TODO: 2022-02-14T00:50:24AEDT effective-c++, effective-c++, 47, example-2, deriving from std::iterator (a deprecated technique?) being something a dedicated example (might be wanted for), (also the new technique) (and what else is derived from to provide type information (and if that's an old technique (again) what are the modern alternatives)?)
//	Ongoing: 2022-02-14T00:57:36AEDT (does) book example seek to achieve same result as inheriting from std::iterator (i.e: defining 'typedef input_iterator_tag iterator_category', (with 'iterator_traits' (a struct in the standard library?) (coming into play) (being specialized by us) ('iterator_traits<IterT>::iterator_category' (being a thing known at compilation))))
    // member typedefs provided through inheriting from std::iterator
    class iterator: public std::iterator<
                        std::input_iterator_tag,   // iterator_category
                        long,                      // value_type
                        long,                      // difference_type
                        const long*,               // pointer
                        long                       // reference		//	Ongoing: 2022-02-14T00:47:51AEDT 'long' not 'long&' as reference type -> correct <here/elsewhere>?
                                      >
	{
        long num = FROM;
    public:
        explicit iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        reference operator*() const {return num;}
    };
    iterator begin() {return iterator(FROM);}
    iterator end() {return iterator(TO >= FROM? TO+1 : TO-1);}
};


//	From: example-book
//	C++ provides a conditional construct for types that is evaluated during compilation: overloading.
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag) {
	cout << "doAdvance, random_access_iterator_tag\n";
	iter += d;
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag) {
	cout << "doAdvance, bidirectional_iterator_tag\n";
	if (d >= 0) { while (d--) ++iter; }
	else { while (d++) --iter; }
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag) {
	cout << "doAdvance, input_iterator_tag (or forward_iterator_tag)\n";
	if (d < 0) { throw std::out_of_range("Negative distance"); }
	while (d--) ++iter;
}
//	because forward_iterator_tag inherits from input_iterator_tag, that version of doAdvance will also handle forward iterators.
//	Ongoing: 2022-02-13T19:15:36AEDT 'iterator_category()' is now a function?
template<typename IterT, typename DistT>
void advance_i(IterT& iter, DistT d) {
	doAdvance(iter, d, typename std::iterator_traits<IterT>::iterator_category());
}

//	Ongoing: 2022-02-14T01:08:01AEDT function vs non-function version of std::iterator_traits<IterT>::iterator_category?

int main()
{
	auto r1 = Range<15,25>();
	auto itr = std::find(r1.begin(), r1.end(), 18);
	cout << "*itr=(" << *itr << ")\n";
	for (auto l: Range<3,5>()) {
		cout << l << " ";
	
	} 
	cout << "\n";
	cout << "\n";

	//	Our advance_i() works for 'Range', which inherits from std::iterator 
	advance_i(itr, 3);


	std::iterator_traits<Range<0,5>::iterator>::value_type x1 = 5;		//	int x1 = 5;

	std::iterator_traits<vector<int>::iterator>::value_type x2 = 7;		//	int x2 = 7;

	//cout << std::iterator_traits<Range<0,5>::iterator>::iterator_category() << "\n";
	//	Ongoing: 2022-02-14T01:14:20AEDT use 'iterator_category' <and/or> 'iterator_category()' (for something) (for, 1: our type, 2: a default type, 3: our 'new-c++' type)



	return 0;
}

