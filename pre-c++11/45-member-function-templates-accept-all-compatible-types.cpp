#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Smart pointers act like pointers, but provide reference counting memory management. Iterators into std containers (are almost always smart pointers) with additional functionality for moving to the next/previous element.

class Top {};
class Middle: public Top {};
class Bottom: public Middle {};

template<typename T>
class SmartPtr {
public:
	//	a naive constructor: support creation from raw pointers only
	explicit SmartPtr(T* realPtr)		//	smart pointers are typically initialized by raw pointers
		: ptr(realPtr)
	{}	

	//	Generalized (Template) Copy Constructor:
	//	Emulating such implicit raw-pointer conversion in user-defined smartpointer classes is tricky. In principle, there might be an unlimited number of conversions, which we cannot practically implement ourselves. To support all the possible conversions, we need a generalized copy constructor (note it is not declared 'explicit', conversion between raw pointers is implicit and we wish to emulate this).
	//	This will compile if used only where there is an implicit conversion U* -> T*
	template<typename U>
	SmartPtr(const SmartPtr<U>& other) 
		: ptr(other.get())
	{}

	//	member function templates may also be used to support assignment from arbitrary supported types
	template<typename U>
	SmartPtr& operator=(SmartPtr<U> const& r) {
		ptr = r.get();
		return *this;
	}

	T* get() const { return ptr; }

private:
	T* ptr;
};

//	Example: Construct from any compatible raw pointer, shared_ptr, weak_ptr, or auto_ptr, Assign from any compatible shared_ptr. (Note the only non-explicit constructor is the generalized copy constructor: the only permitted implicit conversion is between instances of my_shared_ptr).
template<class T>
class my_shared_ptr {
public:
template<class U> explicit my_shared_ptr(U *p);
template<class U> explicit my_shared_ptr(shared_ptr<U> const& r);
template<class U> explicit my_shared_ptr(weak_ptr<U> const& r);
template<class U> my_shared_ptr(my_shared_ptr<U> const& r);
template<class U> my_shared_ptr& operator=(shared_ptr<U> const& r);
//	Declaring generalized constructors/assignments doesn't stop the default constructors/assignments being generated if needed. For full control over our class, we may either delete these, or define our own, otherwise we accept their defaults (in which case it is still better to specify '= default' explicitly).
explicit my_shared_ptr(my_shared_ptr const& r);			//	default copy construction
explicit my_shared_ptr(my_shared_ptr&& r);				//	default move construction
my_shared_ptr& operator=(my_shared_ptr const& r);		//	default copy assignment
my_shared_ptr& operator=(my_shared_ptr&& r);			// 	default move assignment
//	...
};

int main()
{
	//	Something raw Pointers do well is support implicit conversion. Derived class pointers convert into Base class pointers, non-const object pointers convert into const object pointers, <ect>.
	Top *pt1 = new Middle;			//	Convert Middle* -> Top*
	Top *pt2 = new Bottom;			//	Convert Bottom* -> Top*
	//Bottom *pt3 = new Top;		//	cannot Convert Top* -> Bottom*
	const Top *pct2 = pt1;			//	Convert Top* -> const Top*
	delete pt1; pt1 = nullptr; pct2 = nullptr;
	delete pt2; pt2 = nullptr;


	//	Ongoing: 2022-02-13T00:29:29AEDT smart pointer <implicit> conversion (Derived* -> Base*)
	//	Demonstrate same conversions with shared_ptr
	shared_ptr<Top> sp1 = shared_ptr<Middle>(new Middle);
	shared_ptr<Top> sp2 = shared_ptr<Bottom>(new Bottom);
	shared_ptr<const Top> sp3 = sp1;
	//	Ongoing: 2022-02-13T00:40:50AEDT is 'shared_ptr<Top> sp1 = shared_ptr<Middle>(new Middle)' different from 'shared_ptr<Top> sp1(new Middle)'?
	unique_ptr<Top> ut1(new Middle);						//	<(Convert Middle* -> Top* ?)>
	unique_ptr<Top> ut2(new Bottom);						//	<(Convert Bottom* -> Top* ?)>
	unique_ptr<const Top> uct2(move(ut2));					//	<(Convert Top* -> const Top* ?)>

	SmartPtr<Top> msp1 = SmartPtr<Middle>(new Middle);
	SmartPtr<Top> msp2 = SmartPtr<Bottom>(new Bottom);
	//SmartPtr<Bottom> msp3 = SmartPtr<Top>(new Top);		//	cannot convert Top* -> Bottom
	SmartPtr<const Top> msp4 = SmartPtr<Top>(new Top);
	msp1 = SmartPtr<Top>(nullptr);
	msp2 = SmartPtr<Top>(nullptr);
	msp4 = SmartPtr<Top>(nullptr);

	return 0;
}

//	Summary:
//		Use member function templates to generate functions that accept all compatible types
//		If declaring member templates for generalized construction or assignment, the normal (non-template) constructors and assignment operators need to be declared or deleted, or else the default versions will be created as needed.

