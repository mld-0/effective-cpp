#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	//	assignment to self is when an object is assigned to itself
	string w;
	w = w;

	//	may be less immediately obvious when assigning array elements (or between multiple pointers to the same object)
	string a[10];
	int i = 3;
	a[i] = a[3];

	//	expect it to happen and handle it correctly

	//	Ongoing: 2022-01-25T04:00:45AEDT declarations (classes, and other) within (function) <bodies/blocks>

	class Bitmap { };
	class Widget_simple {
		public:
			//	...
			//	simplest solution to self-assignment problem: identity test, this solution is not exception safe
			Widget_simple& operator=(const Widget_simple& rhs) {
				if (this == &rhs) return *this;		//	self-assignment, nothing to do (note we still return '*this')
				//	...
				return *this;		//	assignment operator returns '*this', see item 10
			}
		private:
			Bitmap *pb;
	};

	//	Ongoing: 2022-01-25T06:58:16AEDT there is (a lot) more to be said about (the different kinds of) 'swap()' function ... how's about (some kind of) default?
	//	TODO: 2022-01-25T07:03:21AEDT effective-c++, 11-handle-self-assignment, (this being a slightly complicated topic of which <from the book we adapt a half finished example (further)>), how best to do the 'swap()' in 'copy-and-swap' is a somewhat involved subject (swap as member/friend function, implemented best / in terms of a default?) (does copy-and-swap warrent a dedicated example (something the book doesn't make a dedicated item?)) (best copy and swap implements both move and swap (together)?)

	//	the better solution is exception safe assignment ((in this case) exception safety is also self assignment safety) (for exception safety see item 29)
	//	copy-and-swap exception safe solution
	class Widget {
		void swap(Widget& rhs) {		//	exchange data of 'this' and 'rhs', (see item 29 on exception safety)
		}
		//friend void swap(Widget& lhs, Widget& rhs) {
		//	using std::swap;
		//	//swap(
		//}
		Widget& operator=(const Widget& rhs) {
			Widget temp(rhs);
			//swap(*this, temp);
			swap(temp);
			return *this;
		}
	};

	Widget x;

	return 0;
}

//	Summary:
//		
