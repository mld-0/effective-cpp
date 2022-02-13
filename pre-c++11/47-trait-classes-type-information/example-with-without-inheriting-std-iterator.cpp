//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{1

//	LINK: https://en.cppreference.com/w/cpp/iterator/iterator

//	Ongoing: 2022-02-14T01:40:19AEDT (some details) on why std::iterator is being deprecated(?)
//	LINK: https://stackoverflow.com/questions/29108958/why-does-reverse-iterator-doubly-define-its-nested-types

//	The Question:
//	LINK: https://stackoverflow.com/questions/37031805/preparation-for-stditerator-being-deprecated/38103394
//	{{{
//	On March 21st the standards committee voted to approve the deprecation of std::iterator proposed in P0174:
//	The long sequence of void arguments is much less clear to the reader than simply providing the expected typedefs in the class definition itself, which is the approach taken by the current working draft, following the pattern set in c++14
//	Before c++17 inheritance from std::iterator was encouraged to remove the tedium from iterator boilerplate implementation. But the deprecation will require one of these things:
//	1	An iterator boilerplate will now need to include all required typedefs
//	2	Algorithms working with iterators will now need to use auto rather than depending upon the iterator to declare types
//	3	Loki Astari has suggested that std::iterator_traits may be updated to work without inheriting from std::iterator
//	Can someone enlighten me on which of these options I should expect, as I design custom iterators with an eye towards c++17 compatibility?
//	}}}

//	First Answer provides 1) and 2)
//	Second Answer:
//	{{{
//	Option 3 is a strictly more-typing version of Option 1, since you have to write all the same typedefs but additionally wrap iterator_traits<X>.
//	Option 2 is unviable as a solution. You can deduce some types (e.g. reference is just decltype(*it)), but you cannot deduce iterator_category. You cannot differentiate between input_iterator_tag and forward_iterator_tag simply by presence of operations since you cannot reflexively check if the iterator satisfies the multipass guarantee. Additionally, you cannot really distinguish between those and output_iterator_tag if the iterator yields a mutable reference. They will have to be explicitly provided somewhere.
//	That leaves Option 1. Guess we should just get used to writing all the boilerplate. I, for one, welcome our new carpal-tunnel overlords.
//	}}}


//	1)	With std::iterator <(old-c++, deprecated in C++17)>
template<long FROM, long TO>
class Range_i {
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


//	2)	Without std::iterator <(new<ish>-C++, <needs-version?>)>
template<long FROM, long TO>
class Range_ii {
public:
    class iterator {
        long num = FROM;
    public:
        iterator(long _num = 0) : num(_num) {}
        iterator& operator++() {num = TO >= FROM ? num + 1: num - 1; return *this;}
        iterator operator++(int) {iterator retval = *this; ++(*this); return retval;}
        bool operator==(iterator other) const {return num == other.num;}
        bool operator!=(iterator other) const {return !(*this == other);}
        long operator*() {return num;}
        // iterator traits
        using difference_type = long;
        using value_type = long;
        using pointer = const long*;
        using reference = const long&;
        using iterator_category = std::forward_iterator_tag;
    };
    iterator begin() {return FROM;}
    iterator end() {return TO >= FROM? TO+1 : TO-1;}
};

//	3)	Using <old-C++> (but without inheriting from std::iterator)?

//	4)	Using <C++20> <(this new technique is called what?)> <(concepts?)> <(is (this new thing) even relevent? (to our <example ...>))>

//	User specializations may define the member type iterator_concept to one of iterator category tags, to indicate conformance to the iterator concepts.


int main()
{

	std::iterator_traits<Range_i<0,5>::iterator>::value_type x1 = 5;
	std::iterator_traits<Range_ii<0,5>::iterator>::value_type x2 = 5;
	std::iterator_traits<Range_i<0,5>::iterator>::iterator_category c1;
	std::iterator_traits<Range_ii<0,5>::iterator>::iterator_category c2;

	return 0;
}

