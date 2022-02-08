#include <iostream>
#include <string>
#include <vector>
using namespace std;

//	TODO: 2022-02-08T20:20:23AEDT overriding a non-virtual function? (what (actually) happens?)

//	C++ public inheritance <offers/allows>: inheritance of interface, and inheritance of implementation

//	Sometimes, it is desirable for a derived class to inherit:
//		only the interface (declaration) of a member function
//		interface and implementation, but also to override (parts of this) inherited implementation
//		interface and implementation, and not be allowed to override anything

//	Any function that overrides a virtual function is also virtual (whether or not it is declared with the virtual keyword)
//	

//	Member function interfaces are always inherited: Any function that applies to the Base class must also apply to the Derived class

class Shape {		
public:
	virtual ~Shape() = default;					//	Any class with virtual functions should have a virtual destructor <(what about Derived classes?)>
	virtual void draw() const = 0;				//	declaring a pure virtual function makes this an abstract class
	virtual void error(const string& msg) {
		cerr << "Shape::error, msg=(" << msg << ")\n";
	}
	int objectID() const;		//	A non-virtual function is not supposed to behave differently in derived classes
};
//	A pure virtual function must be redeclared by any concrete class that inherits it (they typically have no Base class implementation). That is:
//		The purpouse of declaring a pure virtual function is to have derived classes inherit a function interface only
//		The purpouse of declaring a virtual function with implementation is to have derived classes inherit a function interface and default implementation
//		The purpouse of declaring a non-virtual function is to have derived classes inherit a function interface with a mandatory implementation

//	It is reasonable to demand that all classes derived from 'Shape' to support 'draw()', however we recognize that there is no reasonable default implementation

//	It is possible to provide a definition for a pure virtual function, (that is, provide an implementation for 'Shape::draw()') (but it is still not possible to declare an instance of the class containing the pure virtual function). If we were to define 'Shape::draw()', we could call it through a pointer to a derived class object
void Shape::draw() const {
	cout << "Shape::draw() (questionably useful)\n";
}
class Rectangle: public Shape {
	void draw() const override {
		cout << "Rectangle::draw()\n";
	}
};
class Ellipse: public Shape {
	void draw() const override {
		cout << "Ellipse::draw()\n";
	}
};

class Airport {};

//	It is often desirable to make default behaviour available to Derived classes, but only if they ask for it
//	One way to do this is to make the interface function a pure virtual function, and offer the default implementation as a (non-virtual) protected member function of the Base class
class Airplane_i {
public:
	virtual ~Airplane_i() = default;
	virtual void fly(const Airport& dest) = 0;
protected:
	void defaultFly(const Airport& dest) {
		cout << "Airplane_i::defaultFly()\n";
	}
};
class ModelA_i: public Airplane_i {
	virtual void fly(const Airport& dest) { 
		cout << "ModelA_i::fly()\n";
		defaultFly(dest); 
	}
};
class ModelB_i: public Airplane_i {			//	does not declare 'fly()', cannot create instances
};

//	<(Another is to use the ability to provide a definition to a pure virtual function. (This is similar to the previous solution but avoids having to add another function name to the class))>
class Airplane_ii {
public:
	virtual ~Airplane_ii() = default;
	virtual void fly(const Airport& dest) = 0;
};
void Airplane_ii::fly(const Airport& dest) {
	cout << "Airplane_ii::fly()\n";
}
class ModelA_ii: public Airplane_ii {
public:
	virtual void fly(const Airport& dest) {
		cout << "ModelA_ii::fly()\n";
		Airplane_ii::fly(dest);
	}
};
class ModelB_ii: public Airplane_ii {		//	does not declare 'fly()', cannot create instances
};

//	effective-c++: "it is a beginners mistake to both overuse virtual functions and to refuse to use them"

int main()
{
	//Shape *ps = new Shape;	//	cannot create instance of class with pure virtual function
	unique_ptr<Shape> ps1(new Rectangle);
	ps1->Shape::draw();			//	how to call a pure virtual function
	ps1->draw();				//	calls 'Rectangle::draw()'
	cout << "\n";

	unique_ptr<Airplane_i> pa1(new ModelA_i);
	pa1->fly(Airport());
	//unique_ptr<Airplane_i> pb1(new ModelB_i);		//	cannot declare ModelB_i since it does not implement 'fly()'

	unique_ptr<Airplane_ii> pa2(new ModelA_ii);
	pa1->fly(Airport());
	//unique_ptr<Airplane_ii> pb1(new ModelB_ii);		//	cannot declare ModelB_ii since it does not implement 'fly()'

	return 0;
}

//	Summary:
//		Inheritance of interface is different from inheritance of implementation. 
//		Under public inheritance, Derived always inherts the interface of Base
//		Pure virtual functions signify inheritance of interface only
//		Simple virtual functions specify inheritance of interface with a default implementation
//		Non-virtual functions specify inheritance of interface with a mandatory implementation

