//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <random>
using namespace std;
//	{{{2
//	get_vector_randoms:
//	{{{
vector<int> get_vector_randoms(int size, int min=0, int max=9) {
//	{{{
	//	values generated are in range [min, max] 
    std::uniform_int_distribution<int> distribution(min, max);
    std::default_random_engine generator;
	vector<int> result(size);
	auto gen = [&distribution,&generator]() { return distribution(generator); };
	std::generate(result.begin(), result.end(), gen);
	return result;
}
//	}}}
//	}}}

//	TODO: 2022-03-03T02:32:40AEDT effective-c++, item 25, move-rvalue-refs-forward-universal-refs, implement 'my_make_unique' (which is a variadic template function)

//	Ongoing: 2022-03-03T01:12:56AEDT do forward/move behave differently for primitives vs <objects>
//	Ongoing: 2022-03-01T21:05:31AEDT what is the meaning of the type T in 'forward<T>()'? What about 'T&', 'T&&'?

//	Forwarding references (from item 24):
//	{{{
//	'T&&' has two meanings. It can mean rvalue reference, or it can mean forwarding reference.
//	A forwarding reference is an rvalue reference to a cv-unqualified template parameter. A forwarding reference can behave as either an lvalue reference 'T&' or an rvalue reference 'T&&'. 
//	<(bind to const, bind to volatile)>.
//	If 'T&&' is used in a context without type deduction, it is an rvalue reference not a forwarding reference.
//	Forwarding references, (like all references) must be initialized.
//		If a forwarding reference is initalized with an lvalue, it acts as an lvalue reference
//		If a forwarding reference is initalized with an rvalue, it acts as an rvalue reference
//	}}}

//	An rvalue reference is a variable that is eligible for moving. 
//	A forwarding reference is a variable that *might* be eligible for moving. 
//	Therefore use 'std::forward<T>()' for forwarding references, and 'std::move()' for rvalue references. 
//	Using 'forward' on rvalue references is bad (possibility of type error when instantiating template). 
//	Using 'move' on forwarding references is very bad (possibility of <incorrectly/inadvertently> applying move to lvalue parameter, deleting its values).
//	Moving a variable leaves it in an undefined state. Do not use forward/move variables unless they are done being used.

class Widget {
public:
	//	Example: solution forwarding reference for both lvalues/rvalues
	template<typename T>
	void setName_i(T&& newName) { 
		//	Perfect forwarding leads to very verbose error messages when the wrong type is used. We can use a static_assert to produce a more helpful error message for incorrect types.
		static_assert( std::is_constructible<std::string, T>::value, "n must be able to construct a string" );
		name = std::forward<T>(newName); 
	}
	//	A single parameter that supports both lvalues/rvalues is a neater and more scalable solution

	//	Example: (non-ideal) alternative solution, overloads for both lvalues/rvalues
	//	When passed literal string "asdf", the less efficent lvalue version is called since it is const
	//	It also leaves two functions to be maintained 
	void setName_ii(const string& newName) { name = newName; }
	void setName_ii(string&& newName) { name = move(newName); }
	//	Using overloads like this also does not allow for scalability of the number (and hence combination) of arguments

	//	Example: rvalue reference, use 'move'
	void setName_iii(string&& newName) { name = std::move(newName); }
private:
	string name;
};


//	On the question of scalability: To implement n parameters that can be lvalues/rvalues through overloading would require 2**n functions.
//	Example: my_make_unique
template<class T, class... Args>
unique_ptr<T> my_make_unique(Args&&... args) {
}


//	Example: use variable in function before (not after) passing it with move/forward - once a variable is move/forward-ed, its value is unspecified.
//	<>


//	Example: use move to return a <non-local> rvalue by value (otherwise a copy is performed)
class My_Int {
public:
	My_Int(int x) 
		: value(x)
	{}
	My_Int& operator+=(const My_Int& rhs) {
		cout << "My_Int, operator+=(const My_Int&)=(" << rhs.value << "), this=(" << this->value << ")\n";
		this->value += rhs.value;
		return *this;
	}
	//	Optimized form of 'operator+' only applicable if lhs is an rvalue (implemented in terms of 'operator+=')
	friend My_Int operator+(My_Int&& lhs, const My_Int& rhs) {
		cout << "My_Int, operator+(My_Int&&, const My_Int&)=(" << lhs.value << ", " << rhs.value << ")\n";
		lhs += rhs;			//	calls My_Int::operator+=()
		return std::move(lhs);
	}
	//	Ongoing: 2022-03-03T03:17:42AEDT 'operator+', const-lvalue-references, or pass-by-value? a meaningful difference / reason to chose?
	//	General form of 'operator+' for lvalue
	friend My_Int operator+(const My_Int& lhs, const My_Int& rhs) {
		//	copy-elision makes a move un-necessary?
		cout << "My_Int, operator+(const My_Int&, const My_Int&)=(" << lhs.value << ", " << rhs.value << ")\n";
		return My_Int(lhs.value + rhs.value);
	}
	int value;
};
//	Ongoing: 2022-03-03T03:03:16AEDT <(on the use of 'operator+(My_Int&&, const My_Int&)' -> when is this rvalue-ref, const-lvalue-ref combination used in examples of such an operator? -> (in fact) it is not useable for lvalue arguments? -> is is sufficient to support 'operator+=', but not 'mi1 + mi2'? -> also let us consider the question again, operators that return references/values/either, when/why? (is it binary functions returning by-value, mono functions (other parameter being 'this') return reference to '*this'? -> Is not an 'extra/operators' example item (over)due?)> <('operator+=' is not supported for 'operator+(My_Int&, My_Int&&)')>


//	Ongoing: 2022-03-06T19:54:07AEDT (need to move), (copy-elision not applicable for returning reference by value)?
//	Example: use forward to return a <non-local> forwarding-reference by value (otherwise a copy is performed)
class Fraction {};
template<typename T>
Fraction reduceAndCopy(T&& frac) {
	frac.reduce();
	return std::forward<T>(frac);
}


//	Example: Do not move/forward to return local variables. Either copy-elision is performed, or an implicit 'move()' is applied to the return value, (either way, the compiler provides the optimal solution without move/forward).
//	<(The result of move/forward is a reference, copy-elision cannot be applied to a reference - besides being suboptimal, returning references to local variables is <problematic> <or not a problem so long a return type is by value>?)>
Widget my_make_widget() {
	Widget result;
	return result;	
}


//	Example: Compiler warning, move-ing local variable in return statement prevents copy-elision
//Widget my_make_widget_ii() {
//	Widget result;
//	return move(result);
//}


//	Example: <('forward<T>(x)' and 'forward<decltype(x)>(x)' are both valid, even though T and decltype(x) may be different)>
//	<>


//	Example: <('forward<decltype(x)>(x)' and 'decltype(x)(x)' are equivalent?)>
//	<>


//	Ongoing: 2022-03-06T20:42:53AEDT can construct a vector from a forwarded vector-as-forwarding-ref, cannot construct vector from moved vector-as-forwarding-ref
template<typename T>
void steal_values(T&& values) {
	T temp( std::forward<T>(values) );
	(void) temp;
}
void steal_values_ii(vector<int>&& values) {
	vector<int> temp( std::move(values) );
	(void) temp;
}
void steal_values_ii(vector<int>& values) {
	vector<int> temp( values );
	(void) temp;
}


//	Example: steal value passed as lvalue reference
void steal_values_iii(vector<int>& values) {
	vector<int> temp( move(values) );
	(void) temp;
}


//	Example: do not move/forward values unless they are done being used
template<typename F, typename... Args>
void invoke_n_times(F&& f, size_t count, Args&&... args) {
	while (count-- > 1) {
		std::__invoke(f, args...);
	}
	if (count == 0) {
		std::__invoke(std::forward<F>(f), std::forward<Args>(args)...);
	}
}
//	Ongoing: 2022-03-06T22:44:42AEDT using '__invoke' because 'invoke' produces a YCM error (but not a compiler error?)


//	Example: get_type_name<T>()
//	LINK: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#include <string_view>
template <typename T>
constexpr auto get_type_name() -> std::string_view {
//	{{{
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


int main()
{
	//	Ongoing: 2022-03-19T22:32:59AEDT we have established (we have?) that [...] (there is no use for forward, outside a function with forwarding reference parameters(?)
	//	Ongoing: 2022-03-19T22:32:02AEDT not clear what (this here extensive output) is meant to demonstrate(?)
	//	Ongoing: 2022-03-14T01:43:37AEDT would not some printing of types make the example(s) more <revealing>?
	//	Example: value stealing behaviour when moving
	vector<int> v1 = get_vector_randoms(20, 0, 10);
	vector<int> v2{ move(v1) };
	cout << "v1.size()=(" << v1.size() << ")\n";
	cout << "v2.size()=(" << v2.size() << ")\n";
	string str1 = "asdf";
	string str2{ move(str1) };
	cout << "str1.size()=(" << str1.size() << ")\n";
	cout << "str2.size()=(" << str2.size() << ")\n";
	cout << "\n";

	My_Int mi1{ 5 };
	My_Int mi2{ 7 };
	mi2 += mi1;
	My_Int mi3 = mi1 + mi2;
	cout << "\n";

	vector<int> v3 = get_vector_randoms(20);
	cout << "v3.size()=(" << v3.size() << ")\n";
	steal_values(v3);
	cout << "steal_values(v3), v3.size()=(" << v3.size() << ")\n";
	steal_values(move(v3));
	cout << "steal_values(move(v3)), v3.size()=(" << v3.size() << ")\n";
	cout << "\n";

	auto v4 = get_vector_randoms(20);
	auto&& urefv4 = get_vector_randoms(20);
	cout << get_type_name<decltype(v4)>() << "\n";
	cout << get_type_name<decltype(urefv4)>() << "\n";
	cout << "v4.size()=(" << v4.size() << ")\n";
	cout << "urefv4.size()=(" << urefv4.size() << ")\n";
	cout << "\n";
	steal_values(v4);
	steal_values(urefv4);
	cout << "steal_values, v4.size()=(" << v4.size() << ")\n";
	cout << "steal_values, urefv4.size()=(" << urefv4.size() << ")\n";
	cout << "\n";
	//	<((I'm betting that) using forward on something that isn't a forwarding reference is a mistake?)>
	//steal_values(forward<vector<int>>(v4));
	//steal_values(forward<vector<int>>(urefv4));
	//cout << "steal_values(forward), v4.size()=(" << v4.size() << ")\n";
	//cout << "steal_values(forward), urefv4.size()=(" << urefv4.size() << ")\n";
	steal_values_ii(v4);			//	calls steal_values_ii(vector<int>&)
	steal_values_ii(urefv4);		//	calls steal_values_ii(vector<int>&)
	cout << "steal_values_ii, v4.size()=(" << v4.size() << ")\n";
	cout << "steal_values_ii, urefv4.size()=(" << urefv4.size() << ")\n";
	cout << "\n";
	steal_values(move(v4));
	steal_values(move(urefv4));
	cout << "steal_values(move), v4.size()=(" << v4.size() << ")\n";
	cout << "steal_values(move), urefv4.size()=(" << urefv4.size() << ")\n";
	cout << "\n";

	//	Ongoing: 2022-03-06T23:11:49AEDT (still) looking for different behaviour from forward for lvalue/rvalue
	auto v5 = get_vector_randoms(20);
	auto&& urefv5_i = get_vector_randoms(20);
	auto&& urefv5_ii = v4;
	steal_values(v5);
	steal_values(urefv5_i);
	steal_values(urefv5_ii);
	cout << "steal_values, v5.size()=(" << v5.size() << ")\n";
	cout << "steal_values, v4urefv5_i.size()=(" << urefv5_i.size() << ")\n";
	cout << "steal_values, v4urefv5_ii.size()=(" << urefv5_ii.size() << ")\n";
	cout << "\n";

	//	steal

	Widget w1;
	//w1.setName_i(53);			//	test static_assert type requirements error message

	(void) mi3; (void) urefv4;
	return 0;
}

//	Summary:
//		Apply 'std::move' to rvalue-references and 'std::forward' to forwarding-references (if the caller is finished with them)
//		Do the same thing for rvalue-references and forwarding-references being returned by value.
//		Never apply move/forward to local objects <(if they would otherwise be eligible for <RVO/copy-elision>)>.

