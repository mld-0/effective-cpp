#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Consider: <the obvious solution>, A base class with a virtual 
class GameCharacter_i {
public:
	virtual ~GameCharacter_i() = default;
	//	the fact we haven't used a pure virtual function suggests there is a default implementation
	virtual int health() const;
};

//	Ongoing: 2022-02-09T03:10:13AEDT <rules/behaviour> calling a virtual function <of-a> <from-a> <Derived/Base> class (instance)

//	The Template Method Pattern (via the Non-Virtual Interface Idiom): 
//	(A design principle beginning <with/from> the idea that virtual functions should almost always be private)
//	Having clients call private virtual functions (indirectly) through public non-virtual member functions (note that the 'Template' in 'Template Method' is not a reference to C++'s Templates)
//	The advantage of NVI is pre/post-condition checking, being able to handle whatever the 'before ...' and 'after ...' steps might be, ((and do it) seperately from implementation specific override-able function): either for managing the acquisition and release of data structure locks, or checking class <hierachy> invariants (during development). (Derived class virtual function, defines implementation, Base class virtual function controls when/how implementation is called)
//	NVI Guidelines:
//		1	Prefer to make interfaces non-virtual
//		2	Prefer to make virtual functions private
//		3	Make the Derived class virtual function protected only if it needs to invoke the Base class implementation
//		4	A base class destructor should either by public-and-virtual, or protected-and-non-virtual
class GameCharacter_ii {
public:
	//	Ongoing: 2022-02-08T23:34:01AEDT need to declare a virtual destructor even when the (only) virtual function is private? (when and why do we need to declare a virtual destructor?)
	virtual ~GameCharacter_ii() = default;	
	//	Ongoing: 2022-02-08T23:38:48AEDT (and this) (under what circumstances) works as an interface <for <base/derived> classes and making available the implementation of the Derived class (polymorphically) (transparently) (as-expected)> (and the defintion for invoking virtual member functions <one-ones-self/with-an-implicit-this>)
	//	non-virtual public member interface function, <implemented in terms of private virtual function>, ('wrapper' for the virtual function implementation)
	int health() const { 
		//	before ...
		int result = getHealth(); 
		//	after ...
		return result; 
	} 
private:
	//	implements 'health()', may be redefined by Derived class
	virtual int getHealth() const { cout << "GameCharacter_ii::getHealth()\n"; return 7; }
};
class RobotCharacter_ii: public GameCharacter_ii {
private:
	//	Derived class defines its own implementation for 'health()'
	virtual int getHealth() const override { cout << "RobotCharacter_ii::getHealth()\n"; return 9; }
};
//	(Note that what we are <redefining/overriding> is a private member function (something we didn't have access to)) 
//	(Although it is not strictly speaking necessary that the virtual implementation functions actually be private)
//	(For some class hierachies, the Derived virtual functions will <need to> invoke their Base class counterparts) (and for such calls to be legal, the virtuals need to be protected rather than private)
//	<(Note how the NVI idiom treats each class hierarchy as a tiny (some may like to call trivial) object-oriented framework, where inversion of control (IoC) flow is commonly observed. Frameworks control the flow of the program as opposed to the functions and classes written by the client, which is why it is known as inversion of control. In NVI, the base class controls the program flow.)>


//	Strategy Pattern via <(raw)> Function Pointers:
//	Strategy is a behavioral design pattern that turns a set of behaviors into objects and makes them interchangeable inside original context object. (The original object, called context, holds a reference to a strategy object and delegates it executing the behavior. In order to change the way the context performs its work, other objects may replace the currently linked strategy object with another one).
//	(In this example the 'health()' implementation is provided to the class by a pointer to a function that takes a class reference as argument) <(this being the only (public) access our implementation has to the class)>
//	implementation functions, and forward declaration of class
class GameCharacter_iii;
int getHealthDefault(const GameCharacter_iii& c) { cout << "getHealthDefault\n"; return 3; }
int getHealthDefaultAlt(const GameCharacter_iii& c) { cout << "getHealthDefaultAlt\n"; return 5; }
int ImplementHealthA(const GameCharacter_iii& c) { cout << "ImplementHealthA\n"; return 1; }
int ImplementHealthB(const GameCharacter_iii& c) { cout << "ImplementHealthB\n"; return 4; }
class GameCharacter_iii {
public:
	typedef int(*HealthFunc_T)(const GameCharacter_iii&);
	explicit GameCharacter_iii(HealthFunc_T f = getHealthDefault) 
		: healthFunc(f)
	{}
	int health() const {
		return healthFunc(*this);
	}
private:
	HealthFunc_T healthFunc;
};
class RobotCharacter_iii: public GameCharacter_iii {
public:
	explicit RobotCharacter_iii(HealthFunc_T f = getHealthDefaultAlt)
		: GameCharacter_iii(f)
	{}
};
//	Additional flexibility over NVI: 1) Same character type can have different implementations, 2) Implementation may be changed at runtime. (Downside: implementation function will have no access to private class members (and the only simple solution to this is to weaken the encapsulation of the class: declaring a friend function, or making public part of the interface that would otherwise be hidden)). (Consider whether this makes NVI or Strategy-via-function-pointer the better solution for a given problem).


//	Strategy Pattern via std::function:
//	<replace raw pointer with std::function object>
//	Ongoing: 2022-02-09T03:50:06AEDT (and how do function objects (std::function) work anyway) (a topic for an 'extra/' (with or without (also covering) function pointers))
//	Ongoing: 2022-02-09T03:51:46AEDT on the argument and return types of our function object(?) <(being compatible with 'const GameCharacter_iii&' as a parameter and 'int' as a return type)>
//	<>
//	Ongoing: 2022-02-09T03:58:29AEDT (on the behaviour/use of) 'std::bind()'
//	<(Additional flexibility over Strategy Pattern via Function Pointer: client has <staggeringly?> more flexibility in what can be specified as an implementation function <differing return types, (what else?)> (client can chose any compatiable callable entity to (serve as implementation)))>
//	<(Also ~~getting~~ binding an implicit 'GameLevel' parameter (what are you even doing see-here-now?))>


//	Classic Strategy Pattern:
//	(A more conventional design approach would be): Make the 'health()' implementation function a virtual member function of a seperate HealthCalculation hierachy
//	<>
//	<(recognizable (as) (to people familiar with) <standard> Strategy pattern implementation)>
//	(Allows implementation to be extended through Derived 'HealthCalculation' classes)
//	Ongoing: 2022-02-09T04:07:33AEDT is not 'Strategy' (in this context) the worst f------ name for <>(?)


//	TODO: 2022-02-09T04:25:26AEDT effective-c++, 35-virtual-function-alternatives, what (besides the obvious) is the problem with virtual functions?
//	TODO: 2022-02-09T04:26:07AEDT effective-c++, 35-virtual-function-alternatives, complete examples for 'Strategy Pattern' ((still) being a stupid name?), std::function based and 'Classic' (pointer to (virtual function in) implementation class hierachy)

int main()
{
	GameCharacter_ii g1;
	RobotCharacter_ii r1;
	g1.health();
	r1.health();
	cout << "\n";

	GameCharacter_iii g2;
	GameCharacter_iii g3(ImplementHealthA);
	GameCharacter_iii g4(ImplementHealthB);
	RobotCharacter_iii r2;
	RobotCharacter_iii r3(ImplementHealthA);
	RobotCharacter_iii r4(ImplementHealthB);
	g2.health();
	g3.health();
	g4.health();
	r2.health();
	r3.health();
	r4.health();
	cout << "\n";

	return 0;
}

//	Summary:
//		Consider alternatives to virtual functions when designing a <solution> 
//		Ongoing: 2022-02-09T04:09:11AEDT (making the case for the alternatives to virtual functions) (besides some <small> overhead) (what is the problem with <using> virtual functions?)
//		The Non-virtual interface (NVI) idiom <contends> virtual functions should <usually/almost-always> be private: instead the public interface is provided by a non-virtual function of the Base class, (responsible for <performing> pre/post-checking) (and for calling the implementation (which is a private virtual class in both Derived and Base)). (The virtual implementation, as a class member, has access to other private class members).
//		The Strategy Pattern (turns a set of behaviours into objects and makes them interchangable) <used-here> to provide the implementation as a pointer (to a function that takes a reference to an instance of the class as argument) (a raw function pointer, or the <more flexible> std::function). (This gives us the flexibility to change implementation at runtime, but does not give the implementation access to private class data).
//		<(Replace virtual functions in one hierachy with virtual functions in another hierachy (the <conventional> implementation of the Strategy <design> Pattern))>
//		(this not being a comprehensive list) (consider yet other alternatives)

