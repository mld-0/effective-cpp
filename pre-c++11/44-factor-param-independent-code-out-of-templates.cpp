#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Templates can save duplicate in source code, but can lead to bloat in machine code.
//	<commonality and variability analysis>

//	Given two functions that share common code, conventional wisdom is to move it to a third function, which is called by the previous two. Likewise, given two classes that share common parts, conventional wisdom is to move those parts to a third class, to be shared by the first two through inheritance or composition. (These are examples of explicit code replication)
//	In template code, the replication is implicit, because there is one copy of the template code being instantiated multiple times.

//	Ongoing: 2022-02-12T23:36:18AEDT <when/where> (if ever) is (something like) SquareMatrix<double,5> likely to be a good idea?
//	Ongoing: 2022-02-12T21:12:47AEDT values (i.e: size_t) as <class/function> template parameters?

//	Ongoing: 2022-02-12T23:18:38AEDT (I don't like this example(?))
//	template for an n x n matrix of T objects: takes a type parameter, and a non-type parameter
template<typename T, size_t n>
class SquareMatrix_i {
public:
	void invert() {}
};
//	<(Instantiated once for each pair of T,n)>
//	(This is an example of potential bloat due to non-type parameter 'n', but bloat can occur due to the use of parameter types too)


//	Considering how this might be <factorized>: A Base class declaring functions which take size as a parameter
template<typename T>
class SquareMatrixBase {
protected:
	SquareMatrixBase(size_t n, T* pMem)
		: size(n), pData(pMem)
	{}
	void invert(size_t matrixSize) {}
private:
	size_t size;
	T *pData;
};
//	Derived class inherits size-independent Base implementation
template<typename T, size_t n>
class SquareMatrix: private SquareMatrixBase<T> {
public:
	SquareMatrix()
		: SquareMatrixBase<T>(n,data)
	{}
	void invert() { invert(n); }			//	member functions implemented in terms of base class
private:
	using SquareMatrixBase<T>::invert;		//	make names from template base class available (see item 43)

	//	Ongoing: 2022-02-12T23:47:39AEDT (no call to 'new' or 'malloc') this is not memory that needs releasing(?)
	T data[n*n];
};
//	This avoids a duplication of machine code for each function implementations for each value of 'n' for which the template is initialized (and a reduced code size likely means a more efficient use of cache). However, the versions of such functions with the matrix size hardwired into them by the template are likely to produce better machine code. (The impact of these tradeoffs will have to be measured to be evaluated).
//	(Depending on implementation), Deriving from a size-independent Base class can increase the size of objects (by the size of a pointer in our example). There is also a loss of encapsulation from the sharing of the pointer.
//	The allocate memory will also have to be released <(presumably by the Derived class?)>

//	Some linkers will merge identical function implementations, (but some will not).
//	On most platforms, all types of pointers have the same binary representation, should templates holding pointer types should (often) be able to use a single underlying implementation for each member function.
//	Ongoing: 2022-02-12T23:44:45AEDT <(the book suggests we too do this if code bloat is a concern)> (is this the book telling us to do this) (<in/by> mentioning that the standard library does this) (or telling us that the compiler can do this?)

//	Ongoing: 2022-02-12T23:23:09AEDT book has chosen (item 43) method 2, 'using' declarations to make names from template base class available (I would have used 'this->'?)

int main()
{
	SquareMatrix_i<double,5> sm1;
	sm1.invert();					//	call SquareMatrix<double,5>::invert

	SquareMatrix_i<double,10> sm2;
	sm2.invert();					//	call SquareMatrix<double,10>::invert

	return 0;
}

//	Summary:
//		Templates generate multiple classes and multiple functions, so any template code not dependent on a template parameter causes bloating of machine code.
//		Bloat due to non-type template parameters can often be eliminated by replacing template parameters with function parameters or class data members.
//		Bloat due to type parameters can be reduced by sharing implementations for instantiation types with identical binary representations.

