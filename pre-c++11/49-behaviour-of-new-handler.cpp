#include <iostream>
#include <new>
#include <vector>
#include <string>
using namespace std;

//	Ongoing: 2022-02-15T00:39:04AEDT use of 'new_handler' vs 'new-handler' in <comments/explanations>

//	When operator 'new' fails to satisfy a memory allocation request, it calls a client-specifiable 'new-handler' function (see item 51) (before throwing an exception) (for some older compilers, instead nullptr is returned).
//	To specify this out of memory handling function, call:
//	Ongoing: 2022-02-14T20:05:42AEDT (what is the meaning of) a typedef of the form 'typedef void (*new_handler)();'
typedef void (*new_hander)();
new_hander set_new_handler(new_hander p) throw();
//	the parameter <'p'> is a pointer to the function 'new' should call if it cannot allocate the requested memory. The return value is a pointer to the function in effect for that purpouse before 'set_new_handler()' was called.

//	When new is unable to fulfill a memory request, it calls the new-handler function repeatedly until it can find enough memory. For this reason, (once it has done all it can to make memory available), the new-handler function should either terminate the program directly or set nullptr (in which case an exception is thrown on the next attempt to call new-handler).

//	Example: trival new-handler function
void outOfMem_i() {
	cerr << "outOfMem_i, unable to allocate memory, abort...\n";
	abort();
}

void outOfMem_ii() {
	cerr << "outOfMem_ii, unable to allocate memory, set_new_handler(nullptr)\n";
	std::set_new_handler(nullptr);
}

//	Behaviour of a new-handler:
//		Attempt to make more memory available (in the hope the repeat call to 'new' succeedes)
//		Set a different new-handler function
//		Set a null new-handler function
//		Throw an exception
//		terminate the program


//	Example: implement a custom new_handler for the 'new' and 'new[]' operators for a given class
//	Note the use of non-const static data, something very problematic in a threaded environment
class Widget_i {
private:
	//	Holds an std::new_handler, when destructed, passes it to 'set_new_handler()'
	class NewHandleRestorer { 	
	public:
		explicit NewHandleRestorer(std::new_handler nh)
			: handler(nh)
		{}
		~NewHandleRestorer() { cerr << "~NewHandleRestorer\n"; std::set_new_handler(handler); }
		NewHandleRestorer(const NewHandleRestorer&) = delete;
		NewHandleRestorer& operator=(const NewHandleRestorer&) = delete;
	private:
		std::new_handler handler;
	};
public:
	//	Set the new-handler function used for class new/new[]
	static std::new_handler set_new_handler(std::new_handler p) throw () {
		std::new_handler oldHandler = currentHandler;
		currentHandler = p;
		return oldHandler;
	}
	void* operator new(size_t size) noexcept(false) {		//	throw(std::bad_alloc) in <old-C++>
		cout << "Widget_i::new\n";
		NewHandleRestorer h(std::set_new_handler(currentHandler));
		return ::operator new(size);
	}
	//	Ongoing: 2022-02-14T23:46:08AEDT (is this) correct <implementation/use> of 'new[]'?
	void* operator new[](size_t size) noexcept(false) {	
		cout << "Widget_i::new[]\n";
		NewHandleRestorer h(std::set_new_handler(currentHandler));
		return ::operator new[](size);
	}
private:
	static std::new_handler currentHandler;
};
std::new_handler Widget_i::currentHandler = nullptr;	//	static variable must be defined outside class definition


//	Example: inheritable custom_handler implementation
//	Note that we never use our template parameter 'T' (which is the type inheriting our interface). We declare InheritNewHandler a template so that a seperate static variable 'currentHandler' is substantiated for each type that inherits it. 
//	<(The custom new_handler function is in place <throughout> the construction of the object)>
template<typename T>
class InheritNewHandler {
private:
	//	Holds an std::new_handler, when destructed, passes it to 'set_new_handler()'
	class NewHandleRestorer { 	
	public:
		explicit NewHandleRestorer(std::new_handler nh)
			: handler(nh)
		{}
		~NewHandleRestorer() { cerr << "~NewHandleRestorer\n"; std::set_new_handler(handler); }
		NewHandleRestorer(const NewHandleRestorer&) = delete;
		NewHandleRestorer& operator=(const NewHandleRestorer&) = delete;
	private:
		std::new_handler handler;
	};
public:
	static std::new_handler set_new_handler(std::new_handler p) throw() {
		std::new_handler oldHandler = currentHandler;
		currentHandler = p;
		return oldHandler;
	}
	static void* operator new(size_t size) {
		cout << "InheritNewHandler::new\n";
		NewHandleRestorer h(std::set_new_handler(currentHandler));
		return ::operator new(size);
	}
	//	Ongoing: 2022-02-14T23:46:08AEDT (is this) correct <implementation/use> of 'new[]'?
	static void* operator new[](size_t size) {
		cout << "InheritNewHandler::new[]\n";
		NewHandleRestorer h(std::set_new_handler(currentHandler));
		return ::operator new[](size);
	}
	//	TODO: 2022-02-15T00:22:54AEDT effective-c++, item 49, behaviour of new handler, supporting a nothrow new? (behaviour<s> of nothrow new)
private:
	static std::new_handler currentHandler;
};
template<typename T> std::new_handler InheritNewHandler<T>::currentHandler = nullptr;	//	static variable must be defined outside class defintion

//	Curiously recurring template pattern: (CRTP) Where a Derived class inherits from a template Base class that is substantiated with the same Derived class being declared. (Books suggests the name 'Do it for me').

//	Widget_ii inherits the custom new_handler interface from InheritNewHandler<Widget_ii>
class Widget_ii: public InheritNewHandler<Widget_ii> {};
//	Widget_iii inherits the custom new_handler interface from InheritNewHandler<Widget_iii>
class Widget_iii: public InheritNewHandler<Widget_iii> {};

class Foo {};

int main()
{

	//	Set and restore new_handler
	std::new_handler previousNewHandler = std::set_new_handler(outOfMem_i);
	int *d1 = new int[20];
	delete [] d1;
	std::set_new_handler(previousNewHandler);

	//	assign 'outOfMem_ii()' as new_handler for Widget_i::new/new[]
	Widget_i::set_new_handler(outOfMem_ii);
	Widget_i* a1 = new Widget_i;					
	Widget_i* a2 = new Widget_i[1000L];
	delete a1;
	delete [] a2;
	cout << "\n";

	//	assign 'outOfMem_i()' as new_handler for Widget_ii::new/new[]
	Widget_ii::set_new_handler(outOfMem_ii);
	//	assign 'outOfMem_ii()' as new_handler for Widget_iii::new/new[]
	Widget_iii::set_new_handler(outOfMem_i);

	Widget_ii* a3 = new Widget_ii;					
	Widget_ii* a4 = new Widget_ii[100L];
	delete a3;
	delete [] a4;
	cout << "\n";

	Widget_iii* a5 = new Widget_iii[1000L];
	delete [] a5;
	cout << "\n";

	//	impossible allocations:
	//std::set_new_handler(outOfMem_i);
	//int *pBigData = new int[100000000000000L];
	//delete [] pBigData;
	//Widget_i* a3 = new Widget_i[1000000000000000L];
	//delete [] a3;
	//cout << "\n";

	//	C++ offers an alternative nothrow-ing 'new', which uses failure-yields-null behaviour (this nothrow-ing new still calls new_handler) (and it does not re-try to allocate memory, the new_handler is called <before/after-null-is-returned?>)
	//	Even though a nothrow-ing 'new' will itself not throw, any of the constructors it calls may contain a regular 'new', in which case an exception can still occur during a call to a nothrow-ing 'new'.
	Foo* f1 = new (std::nothrow) Foo;
	Foo* f2 = new (std::nothrow) Foo[100L];
	delete f1;
	delete [] f2;
	int *i1 = new (std::nothrow) int[1000000000000000L];
	if (i1 == nullptr) {
		cout << "i1 == nullptr\n";
	}
	delete [] i1;		//	<((is it) (ever) bad to delete nullptr?)>
	//	Ongoing: 2022-02-15T00:17:08AEDT (usable <for/on> our classes with a custom new_handler?)
	//Widget_i* a6 = new (std::nothrow) Widget_i;		//	not available for our custom new_handler classes?
	//Widget_ii* a7 = new (std::nothrow) Widget_ii;
	//Widget_iii* a8 = new (std::nothrow) Widget_iii;
	cout << "\n";

	cout << "done\n";

	return 0;
}

//	Summary:
//		set_new_handler allows a function, new-handler to be specified to be called when memory allocations cannot be satisifed. After this function completes, the the allocation will be attempted again. If the new-handler function is null, an exception is thrown.
//		The nothrow-ing version of new 'new (std::nowthrow) int[10];' returns null upon failure (the new-handler is called if it is set). Any of the constructors being called in-turn may contain a regular 'new', in which case an exception can result during a call to a nothrow-ing new.

