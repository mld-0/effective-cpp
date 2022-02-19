//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{1

//	LINK: https://www.scs.stanford.edu/~dm/home/papers/c++-new.html

//	Rules:
//	LINK: https://stackoverflow.com/questions/4670782/the-difference-between-delete-and-delete-in-c/4670863
//	{{{
//		If you allocate with malloc(), you use free()
//		If you allocate with new you use delete
//		If you allocate with new[] you use delete[]
//		If you construct with placement-new you call the destructor direct
//		If it makes sense to use vector rather than new[] then use it
//		If it makes sense to use smart-pointers then use them and don't bother to call delete (but you'll still need to call new). The matching delete will be in the smart-pointer.
//	}}}

//	LINK: https://isocpp.org/wiki/faq/freestore-mgmt
//	Ongoing: 2022-02-19T01:34:13AEDT from the c++ guidelines, new vs malloc, (more, on what new/delete are), "make_unique (or make_shared) are nearly always supperior to both new/malloc"

//	TODO: 2022-02-19T01:35:11AEDT effective-c++, extra/new-and-delete, (one of the differences between C/C++), implicitly converting from 'void*' to <other-pointer-type>

//	C++ distinguishes the 'heap' from the 'free-stack'. 'malloc()' allocates on the heap, 'new' allocates on the free stack. <(This distincition is an implementaiton detail, in practice both are frequently one-and-the same)>
//	Ongoing: 2022-02-18T23:31:04AEDT on 'new' being implemented under-the-hood as 'malloc()'?

//	operator new()
//	{{{
//	Throwing: 	void* operator new(std::size_t size)
//		Allocates 'size' bytes of storage, suitably aligned to represent any object of that size, and returns a non-null pointer to the first byte of this block. Throws 'bad_alloc' on failure.

//	Nothrow: 	void* operator new(std::size_t size, const std::nothrow_t& nothrow_value) noexcept
//		As with 'throwing', except it returns a null-pointer on failure instead of throwing an exception.

//	Placement: 	void* operator new(std::size_t size, void* ptr) noexcept
//		Recieves 'ptr', a pointer to already allocated memory of the proper size, and returns it without allocating any additional memory.

//	If a new-handler function has been set with 'set_new_handler()', it is called upon allocation failure before the allocation is attempted again, and only throwing an exception / returning nullptr when the new-handler is null. Any custom new should support this. (see item 51).
//	'::operator new()' can be called explicitly as a regular function, but the keyword 'new' is an operator with very specific behaviour: first the function 'new' is called with size of type specified as the first argument, and if this is successful, an object is initalized or constructed as needed, before finally returning a pointer of the appropriate type. 
//	}}}


//	operator delete()
//	{{{
//	Ordinary:		void operator delete(void* ptr) noexcept
//		Deallocate the memory block pointed to by 'ptr' (if not null). If 'Ordinary' is replaced, 'With Size' must also be replaced (and vice versa).

//	Nothrow:		void operator delete(void* ptr, const std::nothrow_t& nothrow_constant) noexcept
//		Same as 'throwing' <(literally the same?)>. Only called if a nothrow new fails. If 'Nothrow' is replaced, 'Nothrow w/ Size' must also be replaced (and vice versa).

//	Placement:		void operator delete(void* ptr, void* voidptr2) noexcept
//		Does nothing. Only called if a placement new fails.

//	<C++14>
//	With Size:		void operator delete(void* ptr, std::size_t size) noexcept
//		Same as 'ordinary', called with same 'size' value used in allocation, provided as an optimization point

//	Nothrow w/ Size:	void operator delete(void* ptr, std::size_t size, const std::nothrow_t&) noexcept
//		Same as 'nothrow', called with same 'size' value used in allocation, provided as an optimization point


//	'operator delete()' can be called explicitly as a regular function, but the keyword 'delete' will first call the appropriate destructor, the call the deallocation function.

//	The deallocation function for a class object is a member function if it exists, otherwise it is the global function 'operator delete'. (If the delete <expression> is preceded by the scope operator '::operator', only global deallocation functions are considered).

//	Ongoing: 2022-02-19T00:43:09AEDT reasoning/behaviour (as to why) the 'with size' versions of 'delete' must also be replaced (upon replacing the default versions Ordinary/Nothrow)
//	}}}


//	operator new()[]
//	{{{
//	Ongoing: 2022-02-18T23:53:09AEDT is the size recieved by new[] the size of the array being (at least) the size of each object * size of array <(item 50/51/52 tells us we cannot make any assumptions about number of objects from the size given)> (also implying it is called once, not once for each object (and so it does all initialization/construction in one go?)) (so in what way is it different than 'new'?)
//	}}}

//	operator delete()[]
//	{{{
//	}}}


class Widget {
public:
	int val;
	Widget() : val(0) { cout << "Widget::Widget\n"; }
	~Widget() { cout << "Widget::~Widget()\n"; }
};

int main()
{
	//	Placement 'new': Form of new that takes a memory address. (The term is also be used to describe any non-default form of new).
	//	{{{
	//	LINK: https://riptutorial.com/cplusplus/example/9740/placement-new

	//	Example: placement-new recieving a stack variable memory address
	//	The memory for 'byteCharValues' is the same sizeof(int) bytes allocated on the stack as 'intValue'. (Such an address is invalid the moment the function returns).
	int intValue = 0x12345678;
	char *byteCharValues = new (&intValue) char[sizeof(int)];
	cout << "hex(intValue)=(0x" << hex << intValue << ")\n";
	for (int i = sizeof(int)-1; i >= 0; --i) { 
		cout << "hex(byteCharValues[" << i << "])=(0x" << hex << (int) byteCharValues[i] << ")\n";
	}
	cout << "\n";
	//	no need to delete stack variable

	//	Ongoing: 2022-02-19T00:36:26AEDT meaning of '::new' / '::delete' (is this a way to force the built-ins?)
	int* x = ::new int;
	::delete x;

	//	Example: placement-new recieving a free-store memory address
	int *intValues_i = new int[2];
	char *byteCharValues_ii = new (intValues_i) char[2*sizeof(int)];
	byteCharValues_ii[0] = 0x12;
	delete [] intValues_i;

	//	Example: creating and deleting an object using placement-new with free-store address
	char* dynArray_i = new char[256];
	new((void*)dynArray_i) Widget();
	reinterpret_cast<Widget*>(dynArray_i)->~Widget();
	delete [] dynArray_i;
	//	again, this time keeping a pointer to object
	char* dynArray_ii = new char[256];
	Widget* p_dynArray_ii = new((void*)dynArray_ii) Widget();
	p_dynArray_ii->~Widget();
	delete [] dynArray_ii;

	//	Example: creating and deleting an object using placement-new with stack address
	alignas(Widget) char localArray_i[256];
	new((void*)localArray_i) Widget();
	reinterpret_cast<Widget*>(localArray_i)->~Widget();
	//	no need to delete stack variable

	//	}}}



	cout << "done\n";
	return 0;
}

