#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 	// std::copy
#include <cstddef> 		// std::size_t
using namespace std;

//	LINK: https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
//	LINK: https://stackoverflow.com/questions/5695548/public-friend-swap-member-function

//	copy-and-swap is a technique for implementing copy-assignment (and move-assignment), in terms of the copy-constructor, and with a strong no exception guarantee. It involves making a temporary copy of the object with the copy-constructor (or move-constructor), then swapping the contents of the current object with said temporary object. It solves the problem of self-assignment without requiring a test for self-assignment.
//	It is a useful solution when the rule-of-zero cannot be followed. The extra memory required for the temporary object is the tradeoff required for exception safety.
//	copy-and-swap requires that we provide our own swap function for the class in question, (std::swap uses copy-assignment and therefore cannot be used to implement copy-assignment) (it also requires a copy-constructor and a destructor for that class).
//	'swap()' is a non-throwing function that swaps the members of two classes. 

//	TODO: 2022-02-11T00:53:44AEDT effective-c++, extra/copy-and-swap, noexcept, what (special member functions) can/should be marked as noexcept? (making things explict) (vs the compiler being able to deduce noexcept for straightforward functions?)

class dumb_array
{
public:
    // (default) constructor
    dumb_array(std::size_t size = 0)
        : mSize(size),
          mArray(mSize ? new int[mSize]() : nullptr)
    {
		cout << "dumb_array default-constructor, size=(" << size << ")\n";
    }

    // copy-constructor
    dumb_array(const dumb_array& other)
        : mSize(other.mSize),
          mArray(mSize ? new int[mSize] : nullptr)
    {
		cout << "dumb_array copy-constructor\n";
        // note that this is non-throwing because the data types being used <(are non-throwing when copied?)>; more attention to detail with regards to exceptions must be given in a more general case, however
        std::copy(other.mArray, other.mArray + mSize, mArray);
    }

	//	move-constructor
	dumb_array(dumb_array&& rhs) noexcept 	//	a noexcept move-ctor allows certain optimizations by the compiler
		: dumb_array()
	{
		cout << "dumb_array move-constructor\n";
		swap(*this, rhs);
	}

    // destructor
    ~dumb_array()
    {
        delete [] mArray;
    }

	//	because <reasons> <the alternative is problematic> (requires two functions, one to be found via ADL and the other to handle std:: qualifications) ((something to do with) partial template specialization)
	//	swap is best implemented as a <member> friend function
	//	a friend function defined inside a class is: 1) placed in the enclosing namespace, 2) automatically inline, and 3) able to refer to static members of the class without further qualification
	//friend void swap(dumb_array& lhs, dumb_array& rhs) 	//	noexcept?
	friend void swap(dumb_array& lhs, dumb_array& rhs) noexcept
	{
		cout << "dumb_array (friend) swap\n";
		//	call swap for each class member variable, defaulting to std::swap if a specialized implementation cannot be found
		using std::swap;
		swap(lhs.mSize, rhs.mSize);
		swap(lhs.mArray, rhs.mArray);
		//	any new member variables will also need to be swaped
	}

	//	copy/move-assignment
	dumb_array& operator=(dumb_array rhs) //	note: pass by value <copy-constructs for lvalue arguments, move-constructs for rvalue arguments?>
		//	noexcept?
	{
		cout << "dumb_array assignment\n";
		swap(*this, rhs);
		return *this;			//	assignment returns reference to '*this' (see pre-c++11 item 10)
	}

	int& operator[](int index)
	{
		if (index < 0 || index > this->mSize) throw out_of_range("Invalid index");
		return mArray[index];
	}

	std::size_t size() { return this->mSize; }

private:
    std::size_t mSize;
    int* mArray;
};


int main()
{
	const int n = 5;

	cout << "initalize A and assign values:\n";
	dumb_array A(n);
	for (auto i = 0; i < A.size(); ++i) A[i] = i;
	for (auto i = 0; i < A.size(); ++i) cout << A[i] << " ";
	cout << "\n\n";

	cout << "copy-construct B from A\n";
	dumb_array B(A);
	for (auto i = 0; i < A.size(); ++i) cout << A[i] << " ";
	cout << "\n";
	for (auto i = 0; i < B.size(); ++i) cout << B[i] << " ";
	cout << "\n\n";

	cout << "default initalize C and copy-assign from B\n";
	dumb_array C;
	C = B;
	for (auto i = 0; i < C.size(); ++i) cout << C[i] << " ";
	cout << "\n\n";

	cout << "move-construct D from B\n";
	dumb_array D(move(B));
	cout << "B.size()=(" << B.size() << ")\n";
	for (auto i = 0; i < D.size(); ++i) cout << D[i] << " ";
	cout << "\n\n";

	cout << "initalize E and move-assign from D\n";
	dumb_array E;
	E = move(D);
	cout << "D.size()=(" << D.size() << ")\n";
	for (auto i = 0; i < E.size(); ++i) cout << E[i] << " ";
	cout << "\n\n";

	return 0;
}

