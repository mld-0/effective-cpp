#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <list>
using namespace std;

//	Template metaprogramming (TMP) is the process of writing template based C++ programs that execute during compilation. <(The result of TMP is pieces of C++ source code instantiated from templates, which is them compiled as usual)>.
//	TMP makes some things easy that would otherwise be hard or impossible. It can also shift work from runtime to compile-time (and can therefore make compile times much longer).
//	TMP is Turing Complete.

//	Consider: TMP (hello world) factorial calculator
template<unsigned n>
struct Factorial {
	enum { value = n * Factorial<n-1>::value };
};
template<>
struct Factorial<0> {
	enum { value = 1 };
};
//	Each instantiation of the Factorial template is a struct, and each struct uses the enum hack (see item 02) to declare a TMP variable named value.


//	Example: 'advance()', using 'iterator_traits' (see item 47)
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
//	Alternative (non-TMP) flawed implementation for 'advance()' using typeid comparison: even though '+=' is only called if the iterator has been determined to be random access, the template function will not compile if insubstantiated with an iterator type which does not support random access.
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


//	Benefits of TMP:
//		1) Ensure dimensional unit correctness: TMP makes it possible to ensure (during compilation) that all dimensional unit combinations in a program are correct.
//		2) Optimizing matrix operatrions: (item 21 explains that some functions must return new objects), and (item 44 introduces SquareMatrix<T,n>). Consider: 
//			typedef SquareMatrix<double,1000> BigMatrix;
//			BigMatrix m1, m2, m3, m4, m5;
//			<...>
//			BigMatrix result = m1 * m2 * m3 * m4 * m5;
//		This results in the creation of four temporary matricies, one for each call to 'operator*'. Using 'Expression Templates', (an advanced TMP technique) it is possible to eliminate the temporaries and merge the loops (without changing the syntax of the client code above),.
//		3)	Generate custom design pattern implementations: Using a TMP technique 'Policy Based Design', it is possible to create templates representing independent design choices 'policies' that can be combined in arbitrary ways to yield pattern implementations with custom behaviour (for example, allowing a few templates implementing smart pointers to generate hundreds of different smart pointer types). This is the basis for 'Generative Programming'.

int main()
{
	list<int>::iterator iter1;
	//advance_i(iter1, 10);			//	fails to compile

	vector<int>::iterator iter2;
	advance_i(iter2, 10);

	cout << "Factorial<5>::value=(" << Factorial<5>::value << ")\n";
	cout << "Factorial<10>::value=(" << Factorial<10>::value << ")\n";
	cout << "\n";

	return 0;
}

//	Summary:
//		Template metaprogramming (TMP) can shift work from runtime to compile-time, enabling earlier error detection and better runtime performance.
//		TMP can be used to generate custom code based on combinations of policy choices, and it can also be used to avoid generating code inappropriate for particular types.

