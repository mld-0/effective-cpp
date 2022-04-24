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


//	LINK: https://stackoverflow.com/questions/16760334/whats-the-difference-between-raw-pointer-and-weak-ptr
//	shared_ptr is oftern used because it mimics the style of garbage collected languages like C#/Java. There are situations where it is required, but <(unique_ptr used with raw pointers should be prefered?)>
//	<(shared_ptr can be used to create circular references (deadlocks), weak_ptr prevents this)> 
//	<(weak_ptr provides an alternative to raw pointers that provide warning about being left dangling)>
//	<(weak_ptr models a temporary model of ownership)>
//	Ongoing: 2022-03-10T23:47:47AEDT on the role (relationship vis-a-vis ownertship) of weak_ptr


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


	//	TODO: 2022-04-25T00:03:44AEST effective-c++, extra/ownership-with-smart-pointers, Polymorphism/virtual-functions and smart pointers [...] -> (supposedly?)

	//	TODO: 2022-04-25T00:05:26AEST effective-c++, extra/ownership-with-smart-pointers, form/use of a smart pointer vs raw pointer (as parameter type/otherwise?)


	cout << "done\n";
	return 0;
}

