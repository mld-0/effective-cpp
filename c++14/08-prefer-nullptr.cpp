//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	get_type_name<T>()
//	{{{
#include <string_view>
template <typename T>
constexpr auto get_type_name() -> std::string_view {
#if defined(__clang__)
    constexpr auto prefix = std::string_view{"[T = "};
    constexpr auto suffix = "]";
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__GNUC__)
    constexpr auto prefix = std::string_view{"with T = "};
    constexpr auto suffix = "; ";
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__MSC_VER)
    constexpr auto prefix = std::string_view{"get_type_name<"};
    constexpr auto suffix = ">(void)";
    constexpr auto function = std::string_view{__FUNCSIG__};
#else
# error Unsupported compiler
#endif
    const auto start = function.find(prefix) + prefix.size();
    const auto end = function.find(suffix);
    const auto size = end - start;
    return function.substr(start, size);
}
//	}}}

//	'nullptr' has type 'std::nullptr_t'. It implicitly converts to all pointer types. '0' is an int, and 'NULL' is platform specific (long on ARM64).
//	Using 'nullptr' instead of '0' or 'NULL' ensures that the overload meant for pointers ('nullptr_t', or failing that 'void*') is selected. It also ensures the type deduced for template functions is suitable for use as a pointer. It also clarifies that nullptr is being used, whereas '0' may be ambigious.


//	Example: '0', 'NULL', and 'nullptr' may call different overloads of a function
void f_overloads(void* x) { cout << "f_overloads(" << get_type_name<decltype(x)>() << ")=(" << x << ")\n"; }
void f_overloads(int x) { cout << "f_overloads(" << get_type_name<decltype(x)>() << ")=(" << x << ")\n"; }
void f_overloads(long x) { cout << "f_overloads(" << get_type_name<decltype(x)>() << ")=(" << x << ")\n"; }


//	Example: '0', 'NULL', and 'nullptr' have different deduced types for a function template
template<typename T>
void f_template(T x) { cout << "f_template(" << get_type_name<decltype(x)>() << ")=(" << x << ")\n"; }


//	Example: Use 'nullptr_t' as return type for nullptr
void* getNullptr_void() { return static_cast<void*>(0); }
int getNullptr_int() { return 0; }
nullptr_t getNullptr() { return nullptr; }


//	Example: when 'nullptr_t' and 'void*' are overloaded, the former is prefered for 'nullptr', while calls with '0' and 'NULL' are ambigious.
void f_voidOrNullptr(void* x) { cout << "f_voidOrNullptr(" << get_type_name<decltype(x)>() << ")=(" << x << ")\n"; }
void f_voidOrNullptr(nullptr_t x) { cout << "f_voidOrNullptr(" << get_type_name<decltype(x)>() << ")=(" << x << ")\n"; }


//	Example: A function expecting a 'nullptr_t' will accept both '0' and 'NULL'
void only_nullptr(nullptr_t x) { cout << "only_nullptr(" << get_type_name<decltype(x)>() << ")\n"; }


//	Example: Cannot pass '0' (int) or 'NULL' (long) to a function expecting a pointer through a template function
template<typename FuncType, typename PtrType>
decltype(auto) call_func(FuncType func, PtrType ptr) {
	return func(ptr);
}
class Widget {};
int f1(shared_ptr<Widget> x) { return 2; }
double f2(unique_ptr<Widget> x) { return 2.0; }
bool f3(Widget* x) { return false; }


int main()
{
	//	Calling overloaded function:
	f_overloads(nullptr);				//	f_overloads(void*)
	f_overloads(0);					//	f_overloads(int)
	f_overloads(NULL);				//	f_overloads(long)
	cout << "\n";

	//	Calling template function:
	f_template(nullptr);			//	f_template<nullptr_t>()
	f_template(0);					//	f_template<int>()
	f_template(NULL);				//	f_template<long>()
	cout << "\n";

	//	'nullptr_t' is prefered over 'void*' when passed 'nullptr, calls with '0' or 'NULL' are ambigious
	f_voidOrNullptr(nullptr);			//	f_voidOrNullptr(nullptr_t)
	//f_voidOrNullptr(0);				//	error, ambigious
	//f_voidOrNullptr(NULL);			//	error, ambigious
	cout << "\n";

	//	Example: using 'nullptr' instead of '0' or 'NULL' for comparisons can help prevent type errors.
	//	It is a type error to compare nullptr_t with an integer type (although comparison with a literal '0' is valid)
	int zero = 0;
	auto np_void = getNullptr_void();
	auto np_int = getNullptr_int();
	auto np_nullptr = getNullptr();
	if (np_void == 0) {}								
	if (np_void == NULL) {}								
	if (np_void == nullptr) {}						
	//if (np_void == zero) {}						//	error, comparison between pointer and integer
	if (np_int == 0) {}								
	//if (np_int == NULL) {}						//	warning, comparison between NULL and non-pointer
	//if (np_int == nullptr) {}						//	error, can't compare int and nullptr_t
	if (np_int == zero) {}	
	if (np_nullptr == 0) {}
	//if (np_nullptr == NULL) {}					//	warning, comparison between NULL and non-pointer
	if (np_nullptr == nullptr) {}
	//if (np_nullptr == zero) {}					//	error, can't compare int and nullptr_t

	only_nullptr(0);
	only_nullptr(NULL);
	only_nullptr(nullptr);
	cout << "\n";

	//	Example: though we can pass a literal zero where a raw/smart-pointer is expected, doing so by way of a template function involves passing it as an int variable, which is not allowed
	//call_func(f1, 0);							//	error, no conversion from int to shared_ptr<Widget>
	//call_func(f1, NULL);						//	error, no conversion from long to shared_ptr<Widget>
	call_func(f1, nullptr);
	//call_func(f2, 0);							//	error, no conversion from int to unique_ptr<Widget>
	//call_func(f2, NULL);						//	error, no conversion from long to unique_ptr<Widget>
	call_func(f2, nullptr);
	//call_func(f3, 0);							//	error, cannot instansiate 'Widget*' with int
	//call_func(f3, NULL);						//	error, cannot instansiate 'Widget*' with long
	call_func(f3, nullptr);

	return 0;
}

//	Summary:
//		Prefer nullptr to 0 and NULL. This increases clarity and helps prevent errors.
//		Avoid overloading integral and pointer types.

