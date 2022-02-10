#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Never redefine a functions inherited default parameter value
//	(Given (item 36, never redefine a non-virtual function), we limit our discussion to redefining the default parameter value of an inherited virtual function).
//	Virtual functions are dynamically bound, while default parameter values are statically bound.
//	The static type of an object is the type it is declared to have. The static type of a Base class pointer to a Derived object is the Base class. The dynamic type of an object is the type of the object to which it currently refers.
//	The default parameters used for a function call are determined by its static type. (Hence when a Derived virtual function is called through a Base pointer, the default arguments are taken from the Base function)

class Shape {
public:
	enum ShapeColor { Red, Green, Blue };
	virtual ~Shape() = default;
	virtual void draw(ShapeColor c = Red) const = 0;
};
class Rectangle: public Shape {
public:
	virtual void draw(ShapeColor c = Green) const { cout << "Rectangle::draw, c=(" << c << ")\n"; }
};
class Circle: public Shape {
public:
	virtual void draw(ShapeColor c) const { cout << "Circle::draw, c=(" << c << ")\n"; }
};

//	If a virtual function is declared to have default parameters, any Derived classes that redefines that function should declare the same default parameters. This is two levels of problematic: code duplications with dependencies (if the default parameters of Base are changed, the default parameters of all Derived must change).
//	<(The implication is that virtual functions should not have default parameter values)>
//	An alternative is the non-virtual interface idiom (see item 36): have a public non-virtual function with default parameters which calls a private virtual function which the client may redefine.

int main()
{
	unique_ptr<Shape> pc(new Circle);
	unique_ptr<Shape> pr(new Rectangle);

	cout << "Shape::Red=(" << Shape::Red << "), Shape::Green=(" << Shape::Green << "), Shape::Blue=(" << Shape::Blue << ")\n";

	pc->draw();		//	calls Circle::draw(Shape::Red)
	pr->draw();		//	calls Rectangle::draw(Shape::Red)

	return 0;
}

//	Summary:
//		Never redefine an inherited default parameter value, since default parameter values are statically bound, while virtual functions (the only functions that should be redefined) are dynamically bound.
//		Instead of using default parameters in virtual functions, consider the non-virtual interface idiom (see item 36).

