//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	C++ does a poor job of seperating interfaces from implementation

//	(Basically), header files are '#included' and not compiled, whereas source files are compiled and not '#included'

//	Suppose class A uses class B, (B is one of A's dependencies). 
//	Whether it can should be forward declared or included:
//		Do nothing if: 			A makes no reference at all to B
//								The only reference to class B is to declare it a friend
//	
//		Forward declare B if:	A contains a pointer or reference to B
//								One or more functions has a B object/pointer/reference as parameter/return-type 
//	
//		Include B if:			B is a parent class of A
//								A contains a B object
//	
//	Prefer doing nothing, then forward declaring, then including (but don't forward declare the standard library). These rules help prevent circular dependencies


//	Forward declaring templates:
//	Consider:
//	a.h:
//		#include "b.h"
//		template<typename T>
//		class Tem {
//			B b;
//		};
//		typedef Tem<int> A;
//	b.h:
//		//	incorrectly attempt to forward declare A
//		//class A;	//	error, A is a template specialization, not a class
//		//	correct way:
//		template<typename T> class Tem;
//		typedef Tem<int> A;
//	A better solution: alternative headers with forward declarations:
//	a.h:
//		#include "b.h"
//		template<typename T>
//		class Tem {
//		B b;
//		};
//	a_fwd.h:
//		template<typename T> class Tem;
//		typedef Tem<int> A;
//	This allows A to be used <(as a pointer, reference, or function parameter)> by including a header without requiring the entire class definition. (It is important to keep forward headers and the corresponding definition headers <synchronized>)


//	Declaring a class using objects by value requires the definitions of the corresponding classes, typically through imports (as a project should (obviously) not be a single file). This introduces a compilation dependency between these files - if the imported headers change (or any of the files they depend on change), the file containing 'Person_i' must be recompiled as must any files that use it. 
//	{{{
//	date.h:
	class Date {
	public:
		Date(string v) : val(v) {}
		string to_string() const { return val; }
	private:
		string val;
	};
//	address.h:
	class Address {
	public:
		Address(string v) : val(v) {}
		string to_string() const { return val; }
	private:
		string val;
	};
//	person.h:
	//#include "date.h"
	//#include "address.h"
	class Person_i {
	public:
		Person_i(const string& name, const Date& birthday, const Address& addr);
		string name() const;
		string birthDate() const;
		string address() const;
	private:
		string theName;			//	implementation details
		Date theBirthDate;
		Address theAddress;
	};
//	}}}
//	{{{1

//	TODO: 2022-02-04T23:48:05AEDT effective-c++, 31-minimize-compilation-dependencies, cleanup <explanations> for pImpl and interface-concrete compilation-dependecy-minimizing techniques

//	One approach that should not be used is forward-declaring the classes in question - these classes may be different to how they appear (for example, 'string' is a typedef for 'basic_string<char>', not a class), and in anycase one should not manually declare parts of the standard library <(or manually re-declare parts of ones own libraries?)>
//	These forward declarations do not provide the size of the object in question <(so only pointers/references to it may be used)>.

//	This leads to the 'hide the object implementation behind a pointer' (pImpl) technique, splitting a class into its interface and implementation with the former manipulating a pointer to the latter. Clients can then use the interface class without having to recompile when the implementation is changed. That is, replacing dependencies on definitions with dependencies on declarations: making header files self-sufficent where practical, and when its not, depending on declarations in other files not definitions. 

//	Avoid using objects where object references and pointers will do (these can be defined with only a declaration of a type, defining objects requires the definition)
//	Depend on class declarations instead of class definitions where possible (the class declaration is all that is needed to declare a function that recieves or returns the type in question (either by pointer/reference or by value))

//	Ongoing: 2022-02-02T21:41:49AEDT on the role of 'export' in modern C++?

//	Provide seperate header files for declarations and definitions, (for example <iosfwd> provides declarations of iostream components whose definitions are found in other headers) (clients should always import a declaration file instead of (having to) forward declare something themselves (provide seperate header files for declarations and definitions))
//#include "datefwd.h"				//	declares (but doesn't define) class Date
Date today();						//	this declaration is all that is needed to declare functions
void clearAppointments(Date d);

//	TODO: 2022-02-04T21:02:47AEDT effective-c++, 31-minimize-compilation-dependencies, place pImpl and Interface solutions into files-in-dir with make rules, demonstrate which files need be recompiled after changing <various>

//	Goal of examples: The clients of Person are divorced from its implementation (i.e: the Date and Address classes), <(and need not recompile their own code (only relink) when the implementation of Person changes)>

//	Example: pImpl implementation for Person
//	{{{
//	personimpl.h: 
	//#include "date.h"
	//#include "address.h"
	class PersonImpl {
	public:
		PersonImpl(const string& name, const Date& birthday, const Address& addr);
		string name() const;
		string birthDate() const;
		string address() const;
	private:
		string theName;
		Date theBirthDate;
		Address theAddress;
	};
//	personimpl.cpp:
	//#include "personimpl.h"
	PersonImpl::PersonImpl(const string& name, const Date& birthday, const Address& addr)
		: theName(name), theBirthDate(birthday), theAddress(addr)
	{}
	string PersonImpl::name() const { return theName; }
	string PersonImpl::birthDate() const { return theBirthDate.to_string(); }
	string PersonImpl::address() const { return theAddress.to_string(); }
//	person.h:
//	<(why aren't we importing our declaration? is it really <worth-it/necessary> to declare 'personimplfwd'?)>
	class PersonImpl;						//	forward declaration
	class Date;
	class Address;
	class PersonInterface_i {
	public:
		PersonInterface_i(const string& name, const Date& birthday, const Address& addr);
		~PersonInterface_i();				//	must declare destructor, and provide (default) definition in source file
		string name() const;
		string birthDate() const;
		string address() const;
	private:
		unique_ptr<PersonImpl> pImpl;		//	smartpointer to implementation
	};
//	person.cpp:
	//#include "person.h"					//	implementing person class, must include its definition
	//#include "personimpl.h"				//	can't call PersonImpl member functions without definitions
	PersonInterface_i::PersonInterface_i(const string& name, const Date& birthday, const Address& addr)
		: pImpl(new PersonImpl(name, birthday, addr))		//	note the use of 'new'
	{}
	//	if the default deleter for unique_ptr is used, the type used in the unique_ptr must be complete where this deleter is invoked: for this reason we define the default destructor in the cpp file
	PersonInterface_i::~PersonInterface_i() = default;
	string PersonInterface_i::name() const { return pImpl->name(); }
	string PersonInterface_i::birthDate() const { return pImpl->birthDate(); }
	string PersonInterface_i::address() const { return pImpl->address(); }
//	main.cpp:
	//#include "date.h"			
	//#include "address.h"
	//#include "person.h"
//	}}}
//	Cost is the dereferencing and storage of the pointer
//	Client may create and use handle class like a conventional object


//	Example: Interface implementation for Person
//	An interface class is a special kind of abstract base class, with a set of pure virtual functions that define an interface. Clients of this class must program in terms of pointers/references since it is not possible to instantiate classes containing pure virtual functions. Clients of an interface class must have a way to create new objects (a 'factory function', or a 'virtual constructor', generally a static function inside the interface class), which return pointers (preferably smart pointers) to dynamically created objects that implement the interface. The implementation is provided by a concrete class, derived from the interface class
//	Ongoing: 2022-02-04T21:46:43AEDT check declarations / imports / client compiliation dependencies
//	{{{
//	person.h:
	class Date;
	class Address;
	class PersonInterface_ii {
	public:
		virtual ~PersonInterface_ii() = 0;		//	must have pure virtual destructor with (empty) definition
		//	interface functions:
		virtual string name() const = 0;
		virtual string birthDate() const = 0;
		virtual string address() const = 0;
		//	factory function:
		static unique_ptr<PersonInterface_ii> create(const string& name, const Date& birthday, const Address& addr);
	};
//	realperson.h: 
	//#include "date.h"
	//#include "address.h"
	//#include "person.h"		//	required(?)
	class PersonConcrete : public PersonInterface_ii {
	public:
		PersonConcrete(const string& name, const Date& birthday, const Address& addr);
		~PersonConcrete();
		string name() const override;
		string birthDate() const override;
		string address() const override;
	private:
		string theName;
		Date theBirthDate;
		Address theAddress;
	};
//	realperson.cpp:
	//#include "realperson.h"
	PersonConcrete::PersonConcrete(const string& name, const Date& birthday, const Address& addr)
		: theName(name), theBirthDate(birthday), theAddress(addr)
	{}
	PersonConcrete::~PersonConcrete() {}
	string PersonConcrete::name() const { return theName; }
	string PersonConcrete::birthDate() const { return theBirthDate.to_string(); }
	string PersonConcrete::address() const { return theAddress.to_string(); }
//	person.cpp:
	//#include "person.h"
	//#include "realperson.h"
	//	empty definition must be provided for pure virtual destructor
	PersonInterface_ii::~PersonInterface_ii() { }
	//	factory function: implemented in terms of 'PersonConcrete' implementation class
	unique_ptr<PersonInterface_ii> PersonInterface_ii::create(const string& name, const Date& birthday, const Address& addr) {
		//	may potentially return different type of derived object, depending on argument values
		return unique_ptr<PersonInterface_ii>(new PersonConcrete(name, birthday, addr));
	}
//	main.cpp:
	//#include "date.h"			
	//#include "address.h"
	//#include "personinterface.h" (?)
//	}}}
//	Cost is that of resolving and storing virtual function pointers
//	Client must create/manipulate object as smartpointer through factory function

//	Handle/Interface classes should be used when it is necessary to minimize compilation dependencies during development
//	For production code, if necessary, the interface class can be replaced with an implementation class

int main()
{
	PersonInterface_i x("name value", Date("date value"), Address("address value"));
	cout << "x.name()=(" << x.name() << "), x.birthDate()=(" << x.birthDate() << "), x.address()=(" << x.address() << ")\n";

	unique_ptr<PersonInterface_ii> y = PersonInterface_ii::create("name value", Date("date value"), Address("address value"));
	cout << "y->name()=(" << y->name() << "), y->birthDate()=(" << y->birthDate() << "), y->address()=(" << y->address() << ")\n";

	return 0;
}

//	Summary:
//		Minimizing compilation dependencies by depending on declarations instead of definitions. Two approaches are pointer-to-implementation and interface-concrete-inheritance (see above)
//		(For both these approaches) library header files should exist in full and declaration-only forms.

