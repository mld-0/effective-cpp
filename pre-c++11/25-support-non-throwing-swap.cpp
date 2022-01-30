#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	TODO: 2022-01-30T10:47:43AEDT effective-c++, item 25, a better discussion of how best to declare swap (as a dedicated example) (extra/swap.cpp) (for modern/ye-olde c++)
//	TODO: 2022-01-30T15:28:41AEDT effective-c++, item 25, explicit vs partial specialization (and extending std::swap for a custom type (adding to the std namespace))

//	when should 'swap()' be explicitly noexcept?

//	To swap the values of two objects is to give each the others value
//	By default accomplished with 'std::swap()', which is generally akin to:
//namespace std {
//	template<typename T>
//	void swap(T& a, T& b) {
//		T temp(a);
//		a = b;
//		b = a;
//	}
//}
//	This supports all types which implement both copy-construction and copy-assignment
//	For types which consist primarily of a pointer to the real data, swapping of pointers can be much quicker than performing multiple copys. This requires a custom swap function.

//	In general, we are not allowed to alter the contents of std namespace (but we are allowed to totally specialize standard templates, like 'swap()', for types of our own creation
//	<not doing so here?>
//	<can add class definitions of swap to std namespace but not class template definitions?>


//	Implementing swap for a class
class Widget_i {
public:
	void swap(Widget_i& rhs) {
		cerr << "Widget_i member swap\n";
		using std::swap;
		swap(x, rhs.x);
	}
private:
	vector<int> x;	//	lots of data
};
//	Ongoing: 2022-01-30T15:22:36AEDT why might one want to add our custom swap non-member function to namespace std (and specifically for non-template class, but not for template class)? (it is legal to specialize the standard std::swap function templates if the specialization is for a user defined type (i.e: not a standard type or fundamental type) (if one can specialize std::swap it is valid, otherwise not), (and one can't partially specialize function templates))
namespace std {
	void swap(Widget_i& lhs, Widget_i& rhs) {
		cerr << "Widget_i non-member swap\n";
		lhs.swap(rhs);
	}
}

//	Implementing swap for a class template
template<typename T>
class Widget_ii {
public:
	void swap(Widget_ii& rhs) {
		cerr << "Widget_ii member swap\n";
		using std::swap;
		swap(x, rhs.x);
	}
public:
	vector<T> x;	//	lots of data
};
template<typename T>
void swap(Widget_ii<T>& a, Widget_ii<T>& b) {
	cerr << "Widget_ii non-member swap\n";
	a.swap(b);
}


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

//	Explicit specialization <(generally one shouldn't specialize a function template?)>
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
	Widget_i a1;
	Widget_i b1;
	swap(a1, b1);
	Widget_ii<int> a2;
	Widget_ii<int> b2;
	swap(a2, b2);

	Bar x;
	Bar y;
	swap(x, y);

	return 0;
}

//	Summary:
//		Provide a 'swap()' member function when std::swap would be inefficent for a type. It should be non-throwing
//		In addition to the member swap, provide a non-member swap that calls the member <(for classes (not templates), specialize std::swap too)>
//		Before calling 'swap()', employ 'using std::swap;', then call swap (without namespace qualification)
//		<(It is fine to totally specialize std templates for user-defined types, but never try to add something completely new to std)>

