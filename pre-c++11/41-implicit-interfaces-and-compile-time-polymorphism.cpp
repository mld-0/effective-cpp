#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Explicit interface: <(Receiving an object that is publicly Derived from some Base interface class (obeys 'is a' Base) (as a pointer/reference of a base type) and achieves runtime polymorphism through virtual functions. Enforced by compiler by checking class inheritance hierarchy)> 
//	Implicit interface: <(Template function that receives a parameter type, which must support the expression in which it is used (have said operators defined, or have an implicit conversion to a type for which they are). Achieves compile-time polymorphism Through template instantiation and function overload resolution.)>
//	Ongoing: 2022-02-10T21:17:25AEDT how to <view/get/list> the implicit interface for some function template?

//	C++ OOP revolves around explicit interfaces and runtime polymorphism.
class Widget_i {
public:
	Widget_i(int size) { data.reserve(size); for (int i = 0; i < size; ++i) { data.push_back(i); } }
	Widget_i(const Widget_i& w) = default;
	virtual ~Widget_i() = default;
	virtual size_t size() const { return data.size(); }
	virtual void normalize() { cout << "Widget_i::normalize()\n"; for (auto& x: data) { x = 0; } }
	friend void swap(Widget_i& lhs, Widget_i& rhs) { cout << "Widget_i friend swap()\n";  using std::swap; swap(lhs.data, rhs.data); }
	void print() { for (const auto& x: data) { cout << x << " "; } cout << "\n"; }
//private:					//	ideally data should be private, for this example, Derived_i needs access
	vector<int> data;
};
class Derived_i: public Widget_i {
public:
	Derived_i(int size) : Widget_i(size) {}
	virtual void normalize() { cout << "Derived_i::normalize()\n"; for (auto& x: data) { x = 1; } }
};
//	private inheritance -> does not inherit interface -> 'is a' does not apply -> cannot be (implicitly converted in order to be) passed as a reference to 'doProcessing(Widget_i&)' or assigned to a 'Widget_i' pointer
class A: private Widget_i {		
public:
	A(int size) : Widget_i(size) {}
};
void doProcessing(Widget_i& w) {	//	must pass by pointer or reference for polymorphic behaviour
	if (w.size() < 10) {
		//	Ongoing: 2022-02-11T00:31:48AEDT a working drop-in for the unimplement<ed/able> '!= someNastyWidget' as used in book
		//	Ongoing: 2022-02-11T02:13:13AEDT any way to make a copy of w, type-and-all?
		//Widget_i temp(w);		//	calling normalize() on an instance of Widget_i prevents any polymorphic behaviour
		//temp.normalize();
		//swap(temp, w);
		w.normalize();
	} 
}
//	Runtime polymorphism: Anything passed as 'Widget_i& w' support the its interface, that is either a Widget_i, or a class publicly Derived from it ('is a' to it), <(and this is enforced by the type system)>


//	Implicit Interfaces and Compile-time polymorphism: C++ templates and generic programming
template<typename T>
void doProcessingTemplate(T& w) { 		//	<(must still pass by reference/pointer for polymorphic behaviour?)>
	if (w.size() < 10) {
		w.normalize();
	}
}
//	Type 'T' must support 'size()' and 'normalize()', which are its implicit interface, that is the set of expressions that must be valid for a <(specialization of the template)> to compile.
//	Compile-time polymorphism: instantiating function templates with different template parameters leads to different functions being called
//	TODO: 2022-02-11T02:29:42AEDT effective-c++, 41-implicit-interfaces-and-compile-time-polymorphism, rules for converting to a type which supports the functions of the implicit interface


template<typename T>
void doProcessingTemplateRRef(T&& w) { 		//	<(must still pass by reference/pointer for polymorphic behaviour?)>
	if (w.size() < 10) {
		w.normalize();
	}
}
//	Ongoing: 2022-02-11T02:37:29AEDT consider, clients may have overloaded *anything* in the class passed to template function (for example, an overload of the '&&' logical-and operator)
//	Ongoing: 2022-02-11T02:30:57AEDT how passing as a universal reference 'T&&' changes <anything to do with implicit interfaces?> -> <(appears to compile and run fine, what if we move something? the code says 'move()' but the behaviour is as-expected as with it?)>

//	TODO: 2022-02-11T00:47:52AEDT effective-c++, 41-implicit-interfaces-and-compile-time-polymorphism, please debug: 'Widget_i(int size) { data.reserve(size); for (int i = 0; i < size; ++i) { data[i] = i; } }', fails to print, used push_back() instead (which is just as fast if space has been reserved?)

int main()
{
	Widget_i a1(5);
	Widget_i a2(10);
	swap(a1, a2);
	cout << "a1: "; a1.print();
	cout << "a2: "; a2.print();
	doProcessing(a1);
	doProcessing(a2);
	cout << "a1: "; a1.print();
	cout << "a2: "; a2.print();
	Derived_i d1(5);
	doProcessing(d1);
	cout << "d1: "; d1.print();
	cout << "\n";

	A a(3);
	//doProcessing(a);		//	invalid since A inherits from Widget_i privately

	Widget_i a3(5);
	Widget_i a4(10);
	swap(a1, a2);
	doProcessingTemplate(a3);
	doProcessingTemplate(a4);
	cout << "a3: "; a3.print();
	cout << "a4: "; a4.print();
	Derived_i d2(5);
	doProcessing(d2);
	cout << "d2: "; d2.print();
	cout << "\n";

	Widget_i a5(5);
	Derived_i d3(5);
	doProcessingTemplateRRef(move(a5));
	doProcessingTemplateRRef(move(d3));
	cout << "a5: "; a5.print();
	cout << "d3: "; d3.print();
	cout << "\n";

	return 0;
}

//	Summary:
//		Classes support explicit interfaces (function signatures) and runtime polymorphism (virtual functions)
//		Templates support implicit interfaces and compile time polymorphism
//		
