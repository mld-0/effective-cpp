#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	//	Only ctors <allowed> (given) for example class
//	class Widget {
//	public:
//	 Widget(int i, bool b) {} // as before
//	 Widget(int i, double d) {} // as before
//	 Widget(std::initializer_list<long double> il) {} // as before
//	 operator float() const { return 7; } // convert to float
//	};

class Widget {
public:
	//Widget() 					{ cout << "\tdefault-ctor\n"; }
	Widget(const Widget& b) 	{ cout << "\tcopy-ctor\n"; }
	//Widget(Widget&& b) 			{ cout << "\tmove-ctor\n"; }

	Widget(int i, bool b) 		{ cout << "\t(int,bool)=(" << i << "," << b << ")\n"; }
	Widget(int i1, int i2) 		{ cout << "\t(int,int)=(" << i1 << "," << i2 << ")\n"; }
	Widget(int i, double d) 	{ cout << "\t(int,double)=(" << i << "," << d << ")\n"; }

	Widget(std::initializer_list<long double> il) 
	{ cout << "\til=("; for (const auto& x: il) { cout << x << ","; } cout << ")\n"; }

	Widget(const Widget& w, int x) 	{ cout << "\t(Widget,int)=(" << *this << "," << x << ")\n"; }

	//Widget& operator=(const Widget& w) 	{ cout << "\tcopy-assignment\n"; return *this; }
	//Widget& operator=(Widget&& w) 		{ cout << "\tmove-assignment\n"; return *this; }

	operator float() const 	{ return 7.0; }
};

int main()
{
	Widget w1(10, true);	// uses parens, calls (int,bool)
	Widget w2{10, true};	// uses braces, but now calls std::initializer_list ctor (10 and true convert to long double)
	Widget w3(10, 5.0);		// uses parens, calls (int,double)


	
	Widget w4{10, 5.0}; 	// uses braces, but now calls std::initializer_list ctor (10 and 5.0 convert to long double)

	Widget w5(w3);			//	uses copy-ctor


	//	TODO: 2022-02-24T05:25:24AEDT effective-c++, item 07, initialization-parentheses-braces-equals/bookwidget (are we sure) book example is wrong 'Widget w6{w3};' calling copy-ctor instead of initializer_list-ctor (as book claims it should) (is the example all corect / can it be updated to work as expected by some detail missed?)

	//	Book example is wrong? (In C++11 and C++17) (Uses copy-ctor where book claims the initializer_list ctor is invoked)
	Widget w6{w3};			// uses braces, calls std::initializer_list ctor (w4 converts to float, and float converts to long double)

	return 0;
}

