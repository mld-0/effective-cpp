//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <new>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	LINK: https://en.cppreference.com/w/cpp/memory/new/operator_new
//	LINK: https://en.cppreference.com/w/cpp/memory/new/operator_delete

//	TODO: 2022-02-19T01:28:20AEDT effective-c++, item 51, conventions-for-new-and-delete, (in modern-c++) one is required to declare 'size' version of delete when customizing delete (for member/non-member versions?)

//	Ongoing: 2022-02-19T22:23:42AEDT what does (the book) have to say about single vs array forms of new/delete (for member/non-member implementations)
//	Ongoing: 2022-02-15T23:36:43AEDT is overriding a static member function ever going to be a good idea?

//	LINK: https://stackoverflow.com/questions/7194127/how-should-i-write-iso-c-standard-conformant-custom-new-and-delete-operators
//	{{{
//	Requirement #1: It should dynamically allocate at least size bytes of memory and return a pointer to the allocated memory. Quote from the C++ standard, section 3.7.4.1.3:
//	The allocation function attempts to allocate the requested amount of storage. If it is successful, it shall return the address of the start of a block of storage whose length in bytes shall be at least as large as the requested size...
//	The standard further imposes:
//	...The pointer returned shall be suitably aligned so that it can be converted to a pointer of any complete object type and then used to access the object or array in the storage allocated (until the storage is explicitly deallocated by a call to a corresponding deallocation function). Even if the size of the space requested is zero, the request can fail. If the request succeeds, the value returned shall be a non-null pointer value (4.10) p0 different from any previously returned value p1, unless that value p1 was sub-sequently passed to an operator delete.
//	This gives us further important requirements:
//	Requirement #2: The memory allocation function we use (usually malloc() or some other custom allocator) should return a suitably aligned pointer to the allocated memory, which can be converted to a pointer of an complete object type and used to access the object.
//	Requirement #3: Our custom operator new must return a legitimate pointer even when zero bytes are requested.
//	One of the evident requirements that can even be inferred from new prototype is:
//	Requirement #4: If new cannot allocate dynamic memory of the requested size, then it should throw an exception of type std::bad_alloc.
//	But! There is more to that than what meets the eye: If you take a closer look at the new operator documentation (citation from standard follows further down), it states:
//	If set_new_handler has been used to define a new_handler function, this new_handler function is called by the standard default definition of operator new if it cannot allocate the requested storage by its own.
//	}}}
//	LINK: https://stackoverflow.com/questions/4134195/how-do-i-call-the-original-operator-new-if-i-have-overloaded-it
//	{{{
//	[You can't, so do this:]
//	Executes a loop: Within the loop, the function first attempts to allocate the requested storage. Whether the attempt involves a call to the Standard C library function malloc is unspecified.
//	Returns a pointer to the allocated storage if the attempt is successful. Otherwise, if the last argument to set_new_handler() was a null pointer, throw bad_alloc.
//	Otherwise, the function calls the current new_handler (18.4.2.2). If the called function returns, the loop repeats.
//	The loop terminates when an attempt to allocate the requested storage is successful or when a called new_handler function does not return.
//	}}}

//	C++ requires a valid pointer from 'new' when 0 bytes are requested. For this reason, requests for 0 bytes are treated as a request for 1 byte.

//	LINK: https://stackoverflow.com/questions/4061514/c-object-created-with-new-destroyed-with-free-how-bad-is-this
//	On creating with 'new' and releasing with 'free()': Yes it does matter.
//	For memory obtained using new you must use delete.
//	For memory obtained using malloc you must use free.
//	new and malloc may use different data structures internally to keep track of what and where it has allocated memory. So in order to free memory, you have to call that corresponding function that knows about those data structures. It is however generally a bad idea to mix these two types of memory allocation in a piece of code.
//	When calling 'free()', the destructor is not called.

//	<(A custom 'new' using 'malloc()' necesitates a custom 'delete' using 'free()'?)>

//	Ongoing: 2022-02-19T22:43:18AEDT new: The standard library implementations of the nothrow versions (5-8) directly calls the corresponding throwing versions (1-4). The standard library implementation of the throwing array versions (2,4) directly calls the corresponding single-object version (1,3). Thus, replacing the throwing single object allocation functions is sufficient to handle all allocations.
//	Ongoing: 2022-02-19T22:43:45AEDT 


//	custom 'new':
//	Since <C++-11>, all the standard nothrow 'new' implementations call their corresponding throwing versions, and the standard array implementations call their corresponding single-object versions. The consqeuence of this is that replacing 'void* operator new(std::size_t size)' is sufficent to customize all non-member non-placement forms of new. <(BUT IS IT?)>
void* operator new(std::size_t size) {
	cout << "custom operator new\n";
	//	Ongoing: 2022-02-16T00:31:15AEDT <(can't call default new from a custom new (the existance of the custom new makes the default new disappear)?)>
	using namespace std;
	if (size == 0) {
		size = 1;
	}
	while (true) {
		//	attempt to allocate size bytes
		void *p = malloc(size);
		//	if successful, return a pointer to the memory
		if (p) {
			return p;
		}
		//	unsuccessful, find out what the current new-handler is
		std::new_handler globalHandler = set_new_handler(0);
		set_new_handler(globalHandler);
		if (globalHandler) {
			(*globalHandler)();
		} else {
			throw std::bad_alloc();
		}
	}
}
//	The new-handling function cannot be accessed directly, instead 'set_new_handler()' must be called to determine what it is.


//	Ongoing: 2022-02-16T00:55:43AEDT (does anything need to be done to call the destructor?) (not the custom delete function's job?)
//	Format of a custom non-member delete:
void operator delete(void* p) noexcept {
	cout << "custom delete\n";
	free(p);
	//	Ongoing: 2022-02-16T00:40:43AEDT 'free()' vs 'delete'? <- (no way to access default delete from custom delete?)
}

//	custom non-member 'new[]': handled by 'new'
//	custom non-member 'delete[]': handled by 'delete'


//	A custom 'new' in a Base class is inheritied by all Derived classes.
//	Ongoing: 2022-02-19T22:22:51AEDT (It would appear) Defining a custom member single-object new/delete is not sufficent to implement array new/delete from
class Base {
public:
	Base() { cout << "Base ctor\n"; }
	~Base() { cout << "Base dtor\n"; }
	static void* operator new(std::size_t size);

	//	member delete may be declared without the 'size' parameter, but if a version with it is declared it will be prefered (and it will recieve the object size)
	static void operator delete(void *rawMemory, std::size_t size);
	
	static void* operator new[](std::size_t size);
	static void operator delete[](void *rawMemory, std::size_t size);
};
class Derived: public Base {
};
//	If this 'new' wasn't designed with Derived in mind (and it probably wasn't), the best way to handle this is to test whether the allocation is for the expected size, and if it isn't to pass off the request to the default new
//	<(implying any Base class implementing new should perform this check/handoff?)>
void* Base::operator new(std::size_t size) {
	cout << "Base::new\n";
	if (size != sizeof(Base)) {		//	since 'sizeof(Base)' can never be zero, handles any call where size=0
		return ::operator new(size);
	}
	//	...
	return ::operator new(size);
}
//	If implementing 'new' for a class, 'new[]' should also be implemented
//	For 'new[]', it cannot be assumed that the size of each object going into the array is 'sizeof(Base)', and therefore it also cannot be assumed that the number of elements is 'size/sizeof(Base)'. <(It also cannot be assumed that more space is not being allocated to store the size of the array)>
//	C++ guarantees it is always safe to delete a null pointer, a custom delete must honor this guarantee
//	If 'new' checks whether the allocation is for the expected size, and if it isn't to pass off the request to the default version, then 'delete' must do the same thing:
void Base::operator delete(void *rawMemory, std::size_t size) {
	cout << "Base::delete\n";
	if (rawMemory == 0) 
		return;
	if (size != sizeof(Base)) {
		::operator delete(rawMemory);
		return;
	}
	//	...
	//	Ongoing: 2022-02-15T23:39:56AEDT not passing 'size' to default delete?
	//	TODO: 2022-02-15T23:45:13AEDT effective-c++, item 51, conventions for new and delete, when to call (and when is called) the (default) 'delete' with/without the 'size' argument?
	::operator delete(rawMemory);
}
//	The size value passed to delete may be incorrect if the object was Derived from a Base class lacking a virtual destructor

//	Ongoing: 2022-02-19T22:25:18AEDT (the correct way to implement member array new/delete?) (these below being copies of single new/delete) (what does the book say about member (and non-member) array new/delete?)
void* Base::operator new[](std::size_t size) {
	cout << "Base::new[]\n";
	if (size != sizeof(Base)) {		//	since 'sizeof(Base)' can never be zero, handles any call where size=0
		return ::operator new(size);
	}
	//	...
	return ::operator new(size);
}
void Base::operator delete[](void *rawMemory, std::size_t size) {
	cout << "Base::delete[]\n";
	if (rawMemory == 0) 
		return;
	if (size != sizeof(Base)) {
		::operator delete(rawMemory);
		return;
	}
	::operator delete(rawMemory);
}



int main()
{
	Base* b = new Base;
	delete b;
	cout << "\n";

	//Base* b2 = new Base[10000000000000000L];
	//delete [] b2;

	Base* b3 = new Base[2];
	delete [] b3;
	cout << "\n";

	cout << "done\n";

	return 0;
}

//	Summary:

