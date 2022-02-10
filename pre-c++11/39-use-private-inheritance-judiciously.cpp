#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Public inheritance is treated as an 'is a' relationship. Compilers will implicitly convert Derived into Base when necessary for a function call to succeed. The same is <(generally)> not true for private inheritance.
//	Members inherited privately are private members of Derived, even if they were protected or public members of Base.
//	Private Inheritance means: 'is implemented in terms of' (note that this is also the definition of Composition).
//	A class, Derived, might inherit from class Base privately in order to take advantage of features available in Base, not because there is a conceptual relationship between Base and Derived: private inheritance is an implementation technique. (Implementation only should be inherited, not interface).
//	Generally, composition should be prefered to private inheritance. <(Private inheritance becomes necessary when access to protected members and/or the ability to redefine virtual functions is required)> (Or when empty base optimization (EBO) is necessary).

class Person {};
class Student: private Person {};
void eat(const Person& p) {}
void study(const Student& s) {}


class Timer {
public:
	explicit Timer(int tickFrequency);
	virtual void onTick() const; 	//	Why would 'onTick()' be const?
};
class Widget_i: private Timer {
private:
	virtual void onTick() const;	//	ditto: re: const
};

//	Alternatively, consider a mix of public inheritance and containment:
class Widget_ii {
private:
	class WidgetTimer: public Timer {
	public:
		virtual void onTick() const;	//	const?
	};
	WidgetTimer timer;
};
//	Widget_ii's Derived classes have no access to WidgetTimer, and hence cannot inherit from it or redefine its virtual functions. This approach also allows us store WidgetTimer as a pointer and its definition out of Widget_ii, and hence remove Timer as a compilation dependency of Widget_ii (see item 31 regarding compilation depencencies).

//	Empty Base Optimization:
class Empty {};
class HoldsAnInt {
public:
	int x;
};
class HoldsAnIntAndAnEmpty {
public:
	int x;
	Empty e;
};
class HoldsAnIntInhertEmpty: private Empty {
public:
	int x;
};
//	C++ decrees that freestanding objects must have a non-zero size. An empty class has a non-zero size, but when derived or deriving it can have zero size. Therefore:
//		sizeof(Empty)=(1)
//		sizeof(HoldsAnInt)=(4)
//		sizeof(HoldsAnIntAndAnEmpty)=(8)
//		sizeof(HoldsAnIntInhertEmpty)=(4)
//	EBO is generally not applicable to Derived classes with more than one Base class.


int main()
{
	Person p;
	Student s;
	eat(p);
	//eat(s);		//	error, a Student is not a Person

	cout << "sizeof(Empty)=(" << sizeof(Empty) << ")\n";
	cout << "sizeof(HoldsAnInt)=(" << sizeof(HoldsAnInt) << ")\n";
	cout << "sizeof(HoldsAnIntAndAnEmpty)=(" << sizeof(HoldsAnIntAndAnEmpty) << ")\n";
	cout << "sizeof(HoldsAnIntInhertEmpty)=(" << sizeof(HoldsAnIntInhertEmpty) << ")\n";

	return 0;
}

//	Summary:
//		Private inheritance means 'is implemented in terms of'. It is generally an inferior option to Composition (which means the same thing). Consider alternatives before using private inheritance. 
//		Private inheritance allows access to protected base class members, and for virtual member functions to be redefined. It also permits the empty base optimization (EBO).
//		A mix of public inheritance and containment (defining a (private) subclass that publicly inherits the Base class, and using that through an instance/pointer) can be a better alternative to private inheritance.

