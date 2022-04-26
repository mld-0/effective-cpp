//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	LINK: https://ericlavesson.blogspot.com/2013/03/c-ownership-semantics.html

//	TODO: 2022-03-10T23:48:33AEDT C++, extra/ownership-with-pointers, ongoings, cleanup

//	Raw pointers can make it difficult to determine who owns a given resource. 

//	Hierachy using raw pointers
class Buffer_i {};
class Mesh_i {
public:
    Mesh_i(Buffer_i* const buffer) 
		: pbuffer(buffer) 
	{}
	//	In this case, since multiple 'Mesh_i' instances may be created with a single buffer, it would be incorrect for to assume responsibility for its deletion, as this would leave the other instances with dangling pointers.
    ~Mesh_i() = default;
	Buffer_i* pbuffer;
};
class Device_i {
public:
    Buffer_i* CreateBuffer_i() { return new Buffer_i(); }
};


//	Smart-pointers:
//		unique_ptr			move-able, not copy-able. Unique ownership. Generally preferable.
//		shared_ptr			copy-able. Shared ownership
//		weak_ptr			<(?)>
//	Ongoing: 2022-03-10T23:43:09AEDT point of weak_ptr vs raw pointer?

//	Hierachy using unique_ptr and raw pointers
class Buffer_ii {};
class Mesh_ii {
public:
	Mesh_ii(Buffer_ii* buffer)
		: pbuffer(buffer)
	{}
	~Mesh_ii() = default;
	Buffer_ii* pbuffer;
};
class Device_ii {
public:
	unique_ptr<Buffer_ii> CreateBuffer_ii() { return unique_ptr<Buffer_ii>( new Buffer_ii() ); }
};


//	Hierachy using unique_ptr, Buffer_iii owns the unique_ptr
//	<(when a unique_ptr is moved, it is set to nullptr?)>
//	This design does not allow multiple
class Buffer_iii {};
class Mesh_iii {
public:
	//	Ongoing: 2022-03-10T23:36:27AEDT correct to pass a unique_ptr (to be moved) by value?
	Mesh_iii(unique_ptr<Buffer_iii> buffer)
		: upbuffer(move(buffer))
	{}
	unique_ptr<Buffer_iii> upbuffer;
};
class Device_iii {
public:
	unique_ptr<Buffer_iii> CreateBuffer_iii() { return unique_ptr<Buffer_iii>( new Buffer_iii() ); }
};


//	Passing smart pointers in/out of functions:
//	{{{
//	LINK: https://www.internalpointers.com/post/move-smart-pointers-and-out-functions-modern-c	

//	Ongoings:
//	{{{
//	Ongoing: 2022-04-25T01:28:18AEST how to get <the/a> underlying reference <to/of> the object pointed to by a smart pointer (presumedly, that is, getting this reference from the pointer returned by 'get()'?)
//	Ongoing: 2022-04-25T01:26:07AEST (supposedly) one does not need to move to return a unique_ptr (what is going on, is it RVO/Copy-epsilon? (are those different things?))
//	Ongoing: 2022-04-25T01:20:54AEST (consider the question: passing (moving) and returning (moving) a unique_ptr (sounds like a terrible idea), (can it be done atomically/safely?)
//	}}}

//	Ways to pass a smart pointer to a function:
//			f(unique_ptr<T>) 	f(shared_ptr<T>) 	f(weak_ptr<T>)
//			f(unique_ptr<T>&) 	f(shared_ptr<T>&)
//			f(T&)
//			f(T*)
//	Core Guidelines: Take a smart pointer as its parameter only if it examines/manipulates said smartpointer (and alternatively, take a raw pointer/reference to operate only on the underlying object)
//	By value: shared_ptr: pass by value to <lend/share?> ownership. unique_ptr: can only be moved
//	Pass by reference to manipulate the ownership of existing smartpointer
//	Raw pointer/reference: <inspect/use> raw pointer (by default) (where only access to underlying object is needed) (pointer can be null, reference may not)

//	Returning smartpointer:
//	By value: (always?) (unique_ptr only since C++17?) <(is there not an item that discusses use of move for return? (and is the conclusion not, don't do it?))>
//	For the underlying pointer return by 'get()': do use 'delete', and do not use it to create new smartpointers


//	Ongoing: 2022-04-26T19:14:58AEST (return smartpointers by value, without using move) (the rules related to this are what / outlined where?)


//	links (see below), a rabbit hole outlined below (and not really, entirely relevent here?) (but by being, raising questions about the rules for overriding virtual vs non-virtual functions).
//	{{{
//	LINK: https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/
//	LINK: https://stackoverflow.com/questions/1115891/covariant-virtual-functions-and-smart-pointers
//	}}}

//	Covariant return type: Where the return type of a method is replaced by a 'narrower' type when it is overriden by a subclass. The problem: when we introduce smartpointers.
struct BaseCVRT {};
struct DerivedCVRT : public BaseCVRT {};
struct ParentCVRT {
	virtual BaseCVRT* foo();
	//virtual BaseCVRT foo_i();
	//virtual unique_ptr<BaseCVRT> bar();
};
struct ChildCVRT : public ParentCVRT {
	virtual DerivedCVRT* foo() override;
	//	narrowing only valid for pointers, not <objects/instances> by value
	//virtual DerivedCVRT foo_i() override;
	//	error, differing return type for overriding function
	//virtual unique_ptr<DerivedCVRT> bar(); override;
};


//	(A <confusing> solution) [...] (How is this not overriding a non-virtual function) (one cannot do that, right?) -> (or since one cannot use a different return type, except as (a pointer to) a narrower class)
//	(To then get to) C++ doesn't know/allow covariant or contravariant templates <(templates meaning virtual functions?)> (There's no relation between types unique_ptr<Base> and unique_ptr<Derived> the way there is between Base* and Derived*).
//	(The point appears to relate to) (different rules for overriding virtual vs non-virtual functions)
class cloneable {
public:
   virtual ~cloneable() {}
   std::unique_ptr<cloneable> clone() const {
      return std::unique_ptr<cloneable>(this->clone_impl());
   }
private:
   virtual cloneable* clone_impl() const = 0;
};
class concrete : public cloneable {
public:
   std::unique_ptr<concrete> clone() const {
      return std::unique_ptr<concrete>(this->clone_impl());
   }
private:
   virtual concrete* clone_impl() const override {
      return new concrete(*this);
   }
};

//	Curiously Recurring Template Pattern CRTP
//	{{{
//	LINK: https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/
//template <typename Derived, typename Base>
////class clone_inherit<Derived, Base> : public Base {		//	<- this is invalid?
//class clone_inherit : public Base {							//	<- article meant to say this?
//public:
//   std::unique_ptr<Derived> clone() const {
//      return std::unique_ptr<Derived>(static_cast<Derived*>(this->clone_impl()));
//   }
//private:
//   virtual clone_inherit* clone_impl() const override {
//      return new Derived(*this);
//   }
//};
//	}}}

//	Bringing it all together:
////	{{{
/////////////////////////////////////////////////////////////////////////////////
// 
//template <typename T>
//class abstract_method
//{
//};
// 
/////////////////////////////////////////////////////////////////////////////////
// 
//template <typename T>
//class virtual_inherit_from : virtual public T
//{
//   using T::T;
//};
// 
/////////////////////////////////////////////////////////////////////////////////
// 
//template <typename Derived, typename ... Bases>
//class clone_inherit : public Bases...
//{
//public:
//   virtual ~clone_inherit() = default;
//
//   std::unique_ptr<Derived> clone() const
//   {
//      return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
//   }
//
//protected:
//   //         desirable, but impossible in C++17
//   //         see: http://cplusplus.github.io/EWG/ewg-active.html#102
//   // using typename... Bases::Bases;
//
//private:
//   virtual clone_inherit * clone_impl() const override
//   {
//      return new Derived(static_cast<const Derived & >(*this));
//   }
//};
// 
/////////////////////////////////////////////////////////////////////////////////
// 
//template <typename Derived, typename ... Bases>
//class clone_inherit<abstract_method<Derived>, Bases...> : public Bases...
//{
//public:
//   virtual ~clone_inherit() = default;
//
//   std::unique_ptr<Derived> clone() const
//   {
//      return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
//   }
//
//protected:
//   //         desirable, but impossible in C++17
//   //         see: http://cplusplus.github.io/EWG/ewg-active.html#102
//   // using typename... Bases::Bases;
//
//private:
//   virtual clone_inherit * clone_impl() const = 0;
//};
// 
/////////////////////////////////////////////////////////////////////////////////
// 
//template <typename Derived>
//class clone_inherit<Derived>
//{
//public:
//   virtual ~clone_inherit() = default;
//
//   std::unique_ptr<Derived> clone() const
//   {
//      return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
//   }
//
//private:
//   virtual clone_inherit * clone_impl() const override
//   {
//      return new Derived(static_cast<const Derived & >(*this));
//   }
//};
// 
/////////////////////////////////////////////////////////////////////////////////
// 
//template <typename Derived>
//class clone_inherit<abstract_method<Derived>>
//{
//public:
//   virtual ~clone_inherit() = default;
//
//   std::unique_ptr<Derived> clone() const
//   {
//      return std::unique_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
//   }
//
//private:
//   virtual clone_inherit * clone_impl() const = 0;
//};
// 
/////////////////////////////////////////////////////////////////////////////////
//
////	<User/Client> code:
/////////////////////////////////////////////////////////////////////////////////
// 
//class cloneable
//   : public clone_inherit<abstract_method<cloneable>>
//{
//};
// 
/////////////////////////////////////////////////////////////////////////////////
//
//class foo
//   : public clone_inherit<abstract_method<foo>, virtual_inherit_from<cloneable>>
//{
//};
//
/////////////////////////////////////////////////////////////////////////////////
//
//class bar
//   : public clone_inherit<abstract_method<bar>, virtual_inherit_from<cloneable>>
//{
//};
//
/////////////////////////////////////////////////////////////////////////////////
//
//class concrete
//   : public clone_inherit<concrete, foo, bar>
//{
//};
//
/////////////////////////////////////////////////////////////////////////////////
//
////	}}}

//	Ongoing: 2022-04-26T18:51:35AEST (article fluentcpp) goes on to introduce CRTP, and to apply these techniques to cases of multiple and diamond inheritance [...] finishes by noting that the 'whole package' example compiles for gcc/clang, but not visual studio [...] -> the need for this only arises from a need to use virtual functions <(review after outlining rules for overriding non-virtual functions)>

//	}}}


//	shared_ptr is oftern used because it mimics the style of garbage collected languages like C#/Java. There are situations where it is required, but <(unique_ptr used with raw pointers should be prefered?)>
//	<(shared_ptr can be used to create circular references (deadlocks), weak_ptr prevents this)> 
//	<(weak_ptr provides an alternative to raw pointers that provide warning about being left dangling)>
//	<(weak_ptr models a temporary model of ownership)>

//	LINK: https://stackoverflow.com/questions/16760334/whats-the-difference-between-raw-pointer-and-weak-ptr
//	Ongoing: 2022-03-10T23:47:47AEDT on the role (relationship vis-a-vis ownership) of weak_ptr


//	Ongoing: 2022-03-10T23:41:01AEDT provide messages in example ctors/dtors?
//	Ongoing: 2022-03-10T23:49:28AEDT what do shared_ptr/weak_ptr describe ownership and do they warrent an example

int main()
{
	//	Example: raw pointers create ambiguities in ownership. It is not obvious to the client whether 'Mesh_i' is responsible for delete-ing the resource 'buffer'. 
	Device_i* device = new Device_i();
	Buffer_i* buffer = device->CreateBuffer_i();
	Mesh_i* mesh = new Mesh_i(buffer);
	//	deletion: it is not necessarily obvious to the client whether 'Mesh_i' is responsible for delete-ing 'buffer'. If we guess this responsibility incorrectly, we face either a memory leak, or we delete a dangling pointer, which is undefined behaviour.
	delete mesh;
	delete device;
	delete buffer;


	//	Example: using unique_ptr and auto
	auto device2 = unique_ptr<Device_ii>(new Device_ii());
	auto buffer2 = device2->CreateBuffer_ii();
	auto mesh2 = unique_ptr<Mesh_ii>(new Mesh_ii( buffer2.get() ) );
	//	deletion: none


	//	Example: using unique_ptr and auto, transfering ownership of 'buffer3' to 'mesh3'
	auto device3 = unique_ptr<Device_iii>(new Device_iii());
	auto buffer3 = device3->CreateBuffer_iii();
	auto mesh3 = unique_ptr<Mesh_iii>(new Mesh_iii( move(buffer3) ) );
	assert( buffer3.get() == nullptr );			//	note that buffer3 has been set to nullptr
	//	deletion: none

	//	TODO: 2022-04-25T00:06:10AEST effective-c++, extra/ownership-with-smart-pointers, using unique_ptr ctor 'unique_ptr<T>(new T(args))' vs 'make_unique<T>(args)'? (see below)
	//	{{{
	//	LINK: https://stackoverflow.com/questions/22571202/differences-between-stdmake-unique-and-stdunique-ptr-with-new
	//	The motivation behind make_unique is primarily two-fold:
	//	make_unique is safe for creating temporaries, whereas with explicit use of new you have to remember the rule about not using unnamed temporaries.
	//		foo(make_unique<T>(), make_unique<U>()); 					// exception safe
	//		foo(unique_ptr<T>(new T()), unique_ptr<U>(new U())); 		// unsafe*
	//	The addition of make_unique finally means we can tell people to 'never' use new rather than the previous rule to "'never' use new except when you make a unique_ptr".
	//	There's also a third reason:
	//	make_unique does not require redundant type usage. unique_ptr<T>(new T()) -> make_unique<T>()
	//	None of the reasons involve improving runtime efficiency the way using make_shared does (due to avoiding a second allocation, at the cost of potentially higher peak memory usage).
	//	* It is expected that C++17 will include a rule change that means that this is no longer unsafe.
	//	}}}

	//	Use of 'new' is not safe in any <multi-statement-expresssion> <with temporaries> (see item 17). 
	//	For this reason, 'make_unique<T>()' is preferable to 'unique_ptr<T>(new T)':
	auto device4 = make_unique<Device_iii>();
	auto buffer4 = device4->CreateBuffer_iii();
	auto mesh4 = make_unique<Mesh_iii>( move(buffer4) );


	//	TODO: 2022-04-25T00:05:26AEST effective-c++, extra/ownership-with-smart-pointers, form/use of a smart pointer vs raw pointer (as parameter type/otherwise?), (passing smart pointers)

	cout << "done\n";
	return 0;
}

