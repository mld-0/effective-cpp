#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Normal form of 'new' <pre-C++11>
//		void operator new(std::size_t) throw(std::bad_alloc);

//	Corresponds to the normal forms of 'delete'
//		void operator delete(void* rawMemory) throw(); 							//	global scope
//		void operator delete(void* rawMemory, std::size_t size) throw();		//	class scope

//	A 'new' that takes additional parameters (beyond 'size') is known as a placement-new
//		void* operator new(std::size_t, void *pMemory) throw();					//	origional placement new

//	Consider: the use of a parameter-taking 'new'
//		Widget* pw = new (std::cerr) Widget;
//	This performs two actions: a memory allocation and a constructor. If the memory allocation succeedes but the ctor fails, C++ is responsible for undoing the memory allocation. To do this, it invokes the placement-delete that corresponds to the placement-new that was called, that is, the version of 'delete' that takes the same number and type of arguments. If no such delete exists, no delete call is made, and the result is a memory leak.
//	Ongoing: 2022-02-18T00:09:23AEDT This applies to member and non-member forms of placement-new?


class Widget {
public:
	static void* operator new(std::size_t size) { return ::operator new(size); }
	static void operator delete(void *pMemory) throw() { ::operator delete(pMemory); }
	//	support for a placement-new requires support for a placement delete
	static void* operator new(std::size_t size, std::ostream& logStream) { return ::operator new(size); }
	static void operator delete(void *pMemory, std::ostream& logStream) { ::operator delete(pMemory); }
};
//	If a placement 'new' is declared, both normal and placement 'delete' should be declared.
//	If a placement 'new' is delcared for a class, unless normal 'new' is also declared it will be hidden <(any way to make it available, i.e: 'using ::operator new'?)>. (Declaring 'new' in a derived class hides both inherited and default 'new').

//	TODO: 2022-02-18T00:44:01AEDT effective-c++, item 52, placement-new-requires-placement-delete, purpouse of each form of new/delete

//	By default, C++ offers the following forms of 'new' <pre-C++11>:
//		void* operator new(std::size_t) throw(std::bad_alloc)					//	normal new
//		void* operator new(std::size_t, void*) throw()							//	placement new
//		void* operator new(std::size_t, const std::now_throw_t&) throw()		//	nothrow new (see item 49)
//	Declaring any custom 'new' for a class hides these default forms.

//	Ongoing: 2022-02-18T00:46:40AEDT note the inconsistent use of return when calling delete -> deliberate detail? (with return type 'void' (is it even possible to return anything?))

//	Example: class implementing normal forms of 'new' and 'delete'
//	<pre-C++11 exception specifications omitted>
class StandardNewDeleteForms {
public:
	//	normal new/delete
	static void* operator new(std::size_t size) noexcept(false) { return ::operator new(size); }
	static void operator delete(void* pMemory) noexcept { ::operator delete(pMemory); }

	//	Ongoing: 2022-02-18T01:08:58AEDT is the 'noexcept' specifier of the 'new(size,ptr)' function correct? (the others too?)
	//	placement new/delete
	static void* operator new(std::size_t size, void* ptr) noexcept { return ::operator new(size,ptr); }
	static void operator delete(void* pMemory, void*ptr) noexcept { return ::operator delete(pMemory, ptr); }

	//	nothrow new/delete
	static void* operator new(std::size_t size, const std::nothrow_t& nt) noexcept { return ::operator new(size, nt); }
	static void operator delete(void* pMemory, const std::nothrow_t&) noexcept { return ::operator delete(pMemory); }
};

//	Consider: through public inheritance of 'StandardNewDeleteForms', we can make available all the default new/delete forms visible again in Derived after a declaration of some custom new/delete hides them.
class Widget_ii: public StandardNewDeleteForms {
public:
	using StandardNewDeleteForms::operator new;				//	make default forms of new/delete available
	using StandardNewDeleteForms::operator delete;

	//	custom new/delete would have otherwise hidden default forms
	static void* operator new(std::size_t size, std::ostream& logStream) { return ::operator new(size); }
	static void operator delete(void* pMemory, std::ostream& logStream) { return ::operator delete(pMemory); }
};

int main()
{
	//	Note that the placement delete is not invoked automatically just because placement new was used
	Widget* w1 = new (std::cerr) Widget;
	delete w1;									//	invokes non-placement delete

	Widget_ii* w2 = new Widget_ii;
	delete w2;

	return 0;
}

//	TODO: 2022-02-18T01:22:57AEDT effective-c++, item 52, placement-new-requires-placement-delete, (Summary/discussion updated as per the answer to) when a custom 'new' necessitates a custom 'delete' (and vice-versa) (this question presumedly extending to "how do the default new/delete work" (can one even make assumptions about them?), (a custom delete calls the destructor and it just works?), (what are the various forms for) -> best (first/next) addressed in 'extra/new-delete'?)
//	Summary:
//		Whenever one writes a placement-new (implementation of 'new' with additional arguments), one must write the corresponding placement-delete (implementation of 'delete' with corresponding arguments) <(in addition to the default delete (for classes/non-member version too?))>
//		When declaring placement versions of 'new' <and/or> 'delete', be sure not to unintentionally hide the default versions (is it even possible to not hide the default non-member versions when declaring custom verisons?) <(<again> this is applicable to class member versions only?)> 

