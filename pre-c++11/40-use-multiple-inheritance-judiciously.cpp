//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	Use Multiple Inheritance Judiciously
//	(It is a way to complicate designs and potentially introduce ambiguities)
//	LINK: https://isocpp.org/wiki/faq/multiple-inheritance
//	(isocpp, on when/how to use MI):
//		Use multiple inheritance only if doing so will remove if/switch statements from caller code 
//	inheritance is not for code reuse, composition is for code reuse, (the primary purpose for inheritance is (dynamic binding *1) <and/or> to express interface compliance (subtyping) <these are the same/different things?>)
//	*1: using inheritance (regularly) without dynamic binding <(virtual functions)> might indicate wrong-thinking
//	inheritance is mainly a specification technique rather than an implementation technique
//	Try to use abstract-base-classes, especially for multiple-inheritance-diamonds (where <most> classes above the 'join class' (where the diamond converges) are ABCs (classes with little (preferable no) data, and most (preferably all) member functions being pure virtual)). (This helps avoid inheriting data/code along multiple paths, and it encourages proper use of inheritance).
//	Consider the bridge pattern or nested generalization as alternatives to MI.
//	The bridge pattern: Create two distinct hierarchies (example is a Vehicle class hierarchy, where Vehicle contains a pointer to an object from the Engine class hierarchy)
//	Nested generalization: <(Pick one primary hierarchy, and the other as secondary)> (example is Vehicle, which is inherited by LandVehicle and WaterVehicle, and in turn derive other classes from these <and so on>)
//	Goodness Criterion:
//	{{{
//		Here are some of the “goodness criteria,” that is, qualities you might want. In this description, N is the number of geographies and M is the number of power sources:
//		Grow Gracefully: Does the size of the code-base grow gracefully when you add a new geography or power source? If you add a new geography (going from N to N+1), do you need to add one new chunk of code (best), M new chunks of code (worst), or something in between?
//		Low Code Bulk: Is there a reasonably small amount of code bulk? This usually is proportional to ongoing maintenance cost — the more code the more cost, all other things being equal. It is also usually related to the “Grow Gracefully” criteria: in addition to the code bulk of the framework proper, best case there would be N+M chunks of code, worst case there would be N×M chunks of code.
//		Fine Grained Control: Do you have fine granular control over the algorithms and data structures? For example, do you have the option of having a different algorithm and/or data structure for any of the N×M possibilities, or are you stuck with using the same algorithm and/or data structure for all, say, gas powered vehicles?
//		Static Detect Bad Combos: Can you statically (“at compile time”) detect and prevent invalid combinations. For example, suppose for the moment that there are no pedal-powered space vehicles. If someone tries to create a pedal-powered space vehicle, can that be detected at compile time (good), or do we need to detect it at run-time?
//		Polymorphic on Both Sides: Does it let users treat either base class polymorphically? In other words, can you create some user code f() that takes any and all, say, land vehicles (where you can add a new kind of land vehicle without requiring any changes to f()), and also create some other user code g() that takes any and all, say, gas powered vehicles (where you can add a new kind of gas powered vehicle without requiring any changes to g())?
//		Share Common Code: Does it let new combinations share common code from either side? For example, when you create a new kind of gas powered land vehicle, can that new class choose to optionally share code that is common to many gas-powered vehicles and choose to optionally share code is common to many land vehicles?
//	}}}

//	<(On the order of items specified during multiple inheritance)> (Constructors are called in the order they are inherited?) <other details?>

class BorrowableItem {
public:
	void checkOut() { cout << "BorrowableItem::checkOut()\n"; }
};
class ElectronicGaget {
private:
	void checkOut() const { cout << "ElectronicGaget::checkOut()\n"; }
};
class MP3Player: public BorrowableItem, public ElectronicGaget {
};
//	Note that we have declared 'checkOut()' public in BorrowableItem and private in 'ElectronicGaget' (and calling it where only the public option ought to be visible is still considered ambiguous).


//	The Deadly Multiple-Inheritance Diamond: An inheritance hierarchy with more than one path between a Base class and a Derived class.
//	(Such a diamond should be cause to reconsider the design of the class hierarchy).
//	If virtual inheritance is not used, 'IOFile_i' will inherit one copy of 'filename' from each class it inherits from, resulting in an ambiguous name lookup whenever it is used.
class File_i {
public:
	string filename;
};
class InputFile_i: public File_i {};
class OutputFile_i: public File_i {};
class IOFile_i: public InputFile_i, public OutputFile_i {};

//	The solution is virtual inheritance for the classes in the diamond that inherit from the same Base class. From the perspective of correctness, all public inheritance should be virtual (however in practice, virtual inheritance has memory usage and runtime costs). (Generally one should avoid putting data in virtual base classes).
//	Ongoing: 2022-02-10T19:10:50AEDT a virtual base class isn't (actually) marked virtual, is is virtual by way of the fact that one-or-more classes inherit from it virtually(?)
class File_ii {
public:
	string filename;
};
class InputFile_ii: virtual public File_ii {};
class OutputFile_ii: virtual public File_ii {};
class IOFile_ii: public InputFile_ii, public OutputFile_ii {};
//	Ongoing: 2022-02-10T19:38:59AEDT (more <on/about> public inheritance)
//	Ongoing: 2022-02-10T19:50:37AEDT virtual inheritance from differing classes (classes not in a diamond)?


//	An application of multiple inheritance: combining an interface class with an implementation class
//	Ongoing: 2022-02-10T19:59:24AEDT this is a poor example (for MI combining an interface and implementation)
class PersonInterface {
public:
	virtual ~PersonInterface() = default;
	virtual string name() const = 0;
	virtual string address() const = 0;
};
class PersonImplementation {
public:
	virtual ~PersonImplementation() = default;
	virtual const char* theName() const { return "PersonImplementation::theName"; }
	virtual const char* theAddress() const { return "PersonImplementation::theAddress"; }
};
class Person: public PersonInterface, private PersonImplementation {
public:
	virtual ~Person() = default;
	virtual string name() const { return PersonImplementation::theName(); }
	virtual string address() const { return PersonImplementation::theAddress(); }
};


int main()
{
	MP3Player mp;
	//mp.checkOut();					//	error: ambiguous name lookup
	mp.BorrowableItem::checkOut();		//	correct usage
	cout << "\n";

	IOFile_i f1;
	//f.filename = "abc";				//	error: ambiguous name lookup
	f1.InputFile_i::filename = "abc";
	f1.OutputFile_i::filename = "def";

	IOFile_ii f2;
	f2.filename = "abc";

	Person p;
	cout << p.name() << "\n";
	cout << p.address() << "\n";

	return 0;
}

//	Summary:
//		Multiple inheritance is more complex than single inheritance and can introduce ambiguity <other-issues?>
//		Virtual inheritance imposes costs of size, speed, and complexity for initialization and assignment, and is best used when base classes contain no data
//		One application of multiple inheritance is combining an interface class and an implementation class (inheriting from the later privately)

