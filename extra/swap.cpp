#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	As taken from pre-c++11/25-support-non-throwing-swap.cpp

//	LINK: https://stackoverflow.com/questions/6380862/how-to-provide-a-swap-function-for-my-class
//	LINK: https://medium.com/@manoj563125/c-deep-dive-pimpl-idiom-swap-function-and-adl-8e1920ca1b1c
//	Providing swap as per stackoverflow:
//	Proper *use* of swap:
template<typename T>
void foo(T& lhs, T& rhs) {
	using std::swap;	//	default to std::swap
	swap(lhs, rhs);		//	use ADL and find fitting 'swap()', or fall back on std::swap
}

//	Providing a swap function for a class:
namespace Foo {

	//	Either using a friend function:
	class Bar_i {
	public:
		friend void swap(Bar_i& lhs, Bar_i& rhs) {
			//	...
		}
	};

	//	Or using a non-member and a member function:
	class Bar_ii {
	public:
		void swap(Bar_ii& rhs) {
			//	...
		}
	};
	void swap(Bar_ii& lhs, Bar_ii& rhs) {
		lhs.swap(rhs);
	}
}

//	Ongoing: 2022-02-01T20:40:32AEDT why one would define a custom swap in std::
//	Explicit specialization (in std) <(generally one shouldn't specialize a function template?)> <(for classes, not templates)>
class Bar {};
namespace std {		// only allowed to extend namespace std with specalizations
	template<>
	void swap<Bar>(Bar& lhs, Bar& rhs) noexcept {
		cerr << "Bar std::swap specialization\n";
		//	...
	}
}

int main()
{
	return 0;
}

