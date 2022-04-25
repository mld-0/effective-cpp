//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
//	{{{2
//	is_lvalue(T&&), get_type_name<T>(), print_details(T&&)
//	{{{
//	'is_lvalue()', determine is argument is lvalue/rvalue 
template<typename T>
constexpr bool is_lvalue(T&& x) {
//	{{{
	return std::is_lvalue_reference<T>{};
}
//	}}}
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
template<typename T>
void print_details(T&& x) {
//	{{{
	cout << "print_details: type(" << get_type_name<decltype(x)>() << "), is_lvalue(" << is_lvalue(std::forward<T>(x)) << ")\n";
}
//	}}}
//	print_vector_elements(ostream&, const vector<string>&, string=", "):
//	TODO: 2022-03-19T03:24:57AEDT effective-c++, extra/variadic-templates, 'print_vector_elements()', most elegant solution to the 'print vector with delims between elements' problem.
void print_vector_elements(ostream& os, const vector<string>& vec, const string delim=", ") {
//	{{{
	for (auto i = 0; i < vec.size(); ++i) {
		cout << vec[i]; 
		if (i != vec.size() - 1) { cout << delim; }
	}
	cout << "\n";
}
//	}}}
//	}}}
class Widget {};
string delim_print_all = ", ";

//	LINK: https://riptutorial.com/cplusplus/example/3208/iterating-over-a-parameter-pack

//	TODO: 2022-03-20T22:15:41AEDT effective-c++, extra/variadic-templates, function parameter packs as lvalue/rvalue/forwarding references
//	TODO: 2022-03-18T20:23:14AEDT effective-c++, extra/variadic-templates, meaning and use of '...' (consider 'std::forward<Ts>(first)...') (The ... pretty much says "take what's on the left, and for each template parameter, unpack it accordingly.") (see below)
//	LINK: https://stackoverflow.com/questions/2821223/how-would-one-call-stdforward-on-all-arguments-in-a-variadic-function
//	TODO: 2022-02-24T23:05:36AEDT effective-c++, extra/variadic-templates, Continue
//	TODO: 2022-03-02T02:01:14AEDT effective-c++, extra/variadic-templates, (item 24) contains a variadic function template example -> complete extra item example, (and from it) fill in (item 24)
//	Ongoing: 2022-03-02T02:09:40AEDT variadic-functions(/classes), and forwarding references?
//	Ongoing: 2022-03-18T19:08:18AEDT expanding 'get_type_name<T>()' to support template parameter packs?
//	Ongoing: 2022-03-18T19:11:39AEDT 'class...' vs 'typename...'?
//	Ongoing: 2022-03-18T20:27:03AEDT meaning of 'Ts&...', 'Ts&&...'?
//	Ongoing: 2022-03-18T20:17:00AEDT 'Ts...' can be deduced as empty? (But we <still> need either a recursive base case, or a 'constexpr(size...)' check?)
//	Ongoing: 2022-03-20T01:01:12AEDT 'my_adder' -> return each '__PRETTY_FUNCTION__' in a vector?


//	A template parameter pack is a template parameter that accepts zero or more template arguments (non-types, types, or templates). A function parameter pack is a function parameter that accepts zero or more function arguments. A template with at least one parameter pack is called a variadic template
//	Variadic functions resolve types at runtime. Variadic templates resolve types at compile time.


//	Ongoing: 2022-02-24T22:59:04AEDT make 'Syntax' (see below) actually <meaningful/useful>
//	Syntax: Template parameter pack appears in: alias template, class template, variable template, and function template parameter lists
//	{{{
//				type... pack-name						non-type template parameter pack w/ optional name
//				typename|class... pack-name				type-template parameter pack w/ optional name
//				type-constraint... pack-name			<C++20> constrained type emplate parameter pack w/ optional name
//				template<params> class... pack-name		<pre-C++17> Template template parameter pack w/ optional name
//				template<params> typename|class... pack-name	<C++17>
//				pack-name... pack-param-name			A function parameter pack w/ optional name
//				pattern...								Parameter pack expansion
//	}}}


//	Example: 'my_adder()' summing variadic function template. Valid for sequence of arguments which accept the '+' operator. Varidaic functions are <typically?> implemented the same way as recursive functions, with a base and recursive case. 
//	template parameter pack: 'typename... Args'
//	function parameter pack: 'Args... args'
template<typename T>
T my_adder(T x) {
	cout << __PRETTY_FUNCTION__ << "\n";
	return x;
}
template<typename T, typename... Args>
T my_adder(T x, Args... args) {
	cout << __PRETTY_FUNCTION__ << "\n";
	return x + my_adder(args...);
}


//	Ongoing: 2022-03-20T01:26:33AEDT categorization of different <forms/implementations> of 'print_all_.*', (50 unbroken lines = bad).
//	Ongoing: 2022-03-19T02:27:08AEDT <(passing a stream by value - even a thing? passing references to things that cannot be copied?)>
//	Examples: print_all, output each variable recieved to stream, with a delim (where simple to do so, the delim is omitted after the last value). 
//	Example: print_all_i <C++11>
template<typename T>
void print_all_i(ostream& os, const T& first) {
	os << first;
}
template<typename T, typename... Ts>
void print_all_i(ostream& os, const T& first, const Ts&... rest) {
	os << first << delim_print_all;
	print_all_i(os, rest...);
}
//	Example: print_all_ii, different base case, doesn't print trailing seperator. <C++11>
template<typename T>
void print_all_ii(ostream& os, const T& last) {
	os << last;
}
template<typename T, typename... Ts>
void print_all_ii(ostream& os, const T& first, const Ts&... rest) {
	os << first << delim_print_all;
	print_all_ii(os, rest...);
}
//	Example: print_all_iii, <(witchcraft)> <C++11>
//	LINK: https://stackoverflow.com/questions/25680461/variadic-template-pack-expansion/25683817#25683817
//	{{{
//		LINK: https://en.cppreference.com/w/cpp/language/fold
//		With C++17 fold expressions, you can just write
//		((void) bar(std::forward<Args>(args)), ...);
//	}}}
template<typename... Ts>
void print_all_iii(ostream& os, const Ts&... args) {
	using expander = int[];
	(void) expander {0, (void(os << args << delim_print_all), 0)...};
}
//	Example: print_all_iv, <C++17>
template<typename... Ts>
void print_all_iv(ostream& os, const Ts&... args) {
	((os << args << delim_print_all), ...);
}
//	Example: print_all_v, <C++17>
template<typename T, typename... Ts>
void print_all_v(ostream& os, const T& first, const Ts&... rest) {
	os << first;
	//	Ongoing: 2022-03-18T19:46:54AEDT 'constexpr()' used in an if statement cannot be placed in brackets?
	if constexpr(sizeof...(rest) > 0) {
		//	called only if 'rest...' is not empty
		os << delim_print_all;
		print_all_v(os, rest...);
	}
}


//	Ongoing: 2022-03-20T00:59:48AEDT Do we need 'fwd_v_.*' (aren't these for (item 30)?) -> multi-line output is <too-much>
//	Example: fwd_v_i, with 'if constexpr(sizeof...(parms) > 0)'
template<typename T, typename... Ts>
void fwd_v_i(T&& first, Ts&&... params) {
	print_details(std::forward<T>(first));
	if constexpr(sizeof...(params) > 0) {
		fwd_v_i(std::forward<Ts>(params)...);
	}
}
//	Example: fwd_v_ii, with recursive base case
template<typename T>
void fwd_v_ii(T&& first) {
	print_details(std::forward<T>(first));
}
template<typename T, typename... Ts>
void fwd_v_ii(T&& first, Ts&&... params) {
	print_details(std::forward<T>(first));
	fwd_v_ii(std::forward<Ts>(params)...);
}


//	TODO: 2022-03-19T04:24:39AEDT effective-c++, extra/varidic-templates, alternative 'get_types_names<Ts...>()' implementations (not using constexpr/sizeof...?)
//	Ongoing: 2022-03-20T00:58:26AEDT 'get_types_names<Ts...>()' does not use forwarding references (and how could it, even if we wanted to, our only argument is the reference to the vector where we store our result)
//	Ongoing: 2022-03-19T04:37:03AEDT 'get_types_names<Ts...>()', returning not a vector<string>, but a vector of <(something that can be used as a type -> is there not an std::type?)>
//	Ongoing: 2022-03-19T04:16:12AEDT the solution is 'constexpr(sizeof...(Ts) > 0)' (where 'typename... Ts' is a template parameter pack) -> being able to get the sizeof a template parameter pack with this techniques implies its implementation was necessary (for a problem just such as this?) (what alternative varidic function technique could have given us this result here?) [...] (also) we do not need the base case(?), for the last call we are fine with an empty Ts, so long as we don't attempt another recursive instantiation with it.
//	Ongoing: 2022-03-19T03:44:35AEDT how to solve 'get_types_names' with base case recursion <(and 'get_types_name<Ts...>(result)')>
//	Example: 'get_types_name<Ts...>()' varidic parameter pack(?)
template<typename T, typename... Ts>
void get_types_names(vector<string>& result) {
	result.push_back( string(get_type_name<T>()) );
	if constexpr(sizeof...(Ts) > 0) {
		get_types_names<Ts...>(result);
	}
}
template<typename... Ts>
vector<string> get_types_names() {
	vector<string> result;
	get_types_names<Ts...>(result);
	return result;
}


//	Ongoing: 2022-03-18T21:32:09AEDT <('my_make_unique' must(?) be instantiated with T i.e: 'my_make_unique<T>' [...] because we have specified T and not used it as a parameter(?) (is this what the C++ template rules specifiy?))>
//	Example: 'my_make_unique()', approximation of 'make_unique'
template<typename T, typename... Ts>
std::unique_ptr<T> my_make_unique(Ts&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Ts>(args)...) );
}

//	Example: 'my_make_shared()', approximation of 'make_shared'
template<typename T, typename... Ts>
std::shared_ptr<T> my_make_shared(Ts&&... args) {
	return std::shared_ptr<T>(new T(std::forward<Ts>(args)...) );
}



//	TODO: 2022-03-19T04:38:36AEDT effective-c++, extra/variadic-templates, implement 'my_printf<T,Ts...>()'
//	Example: my_printf
template<typename T, typename... Ts>
void my_printf(const char* format, T value, Ts... args) {
	for (; *format != '\0'; ++format) {
		//	Ongoing: 2022-03-18T21:42:58AEDT continue (see implementation below), (also use loop from extra/variadic-functions?)
		//	{{{
        //if ( *format == '%' ) {                              // (6) 
        //   std::cout << value;
        //   myPrintf(format + 1, args ... );                  // (7)
        //   return;
        //}
        //std::cout << *format;  
		//	}}}
	}
}


//	TODO: 2022-03-19T04:39:37AEDT effective-c++, extra/variadic-templates, Example: variadic function template and perfect forwarding
//	Example: variadic function template and perfect forwarding
//	<>



//	Ongoing: 2022-03-20T01:17:41AEDT implementing 'Tuple<Ts...>' -> (something of a clusterf---)
//	TODO: 2022-03-19T04:26:30AEDT effective-c++, extra/variadic-templates, (rudimentary) implementation for 'template<typename... Ts> class Tuple'
//	LINK: https://stackoverflow.com/questions/10014713/build-tuple-using-variadic-templates
//	LINK: https://hackernoon.com/variadic-template-in-c-implementing-unsophisticated-tuple-w8153ump
//	Example: Variadic class template Tuple
template<typename... Ts> 
class Tuple {
};


//	{{{
//	Ongoing: 2022-03-02T02:02:17AEDT <(a variadic function template defines a series of pairs of argument types and values?)>
//	Ongoing: 2022-02-24T22:57:09AEDT can't call function 'foo()' without definition (and when were we getting to how to implement such a function -> is it always with a base case?
//	Variadic function template
template<typename... Types> 
void foo(Types... args) {
}
//	}}}
int main()
{
	//	Use of 'my_adder()':
	auto add1 = my_adder(1, 8, 7);
	cout << "add1=(" << add1 << ")\n";
	auto add2 = my_adder(string("Hello "), string("World"));
	cout << "add2=(" << add2 << ")\n";
	cout << "\n";

	//	TODO: 2022-03-19T04:33:23AEDT effective-c++, extra/variadic-templates, 'print_all_.*' functions, print output only for first function, and for each subsiquent function, verify output is same as previous <(removing any trailing comma first)>
	//	Ongoing: 2022-03-20T01:18:58AEDT verifying output of 'print_all_.*' -> a neater solution for accounting for the trailing comma?
	//	TODO: 2022-03-20T01:10:48AEDT effective-c++, extra/variadic-templates, (how to get) (vector of) 'print_all_.*' variadic template function (as a function pointer / std::function / callable-thing-I-can-put-in-a-vector) (and from there), (what is a type we can use to point to all of them, that is, how to put pointers to each in a single vector)
	//	Usage: 'print_all_.*'
	cout << "print_all_i: ";
	stringstream ss;
	print_all_i(ss, 1, 2, 3, 8, 7, "53");
	string result_print_all_i = ss.str();
	ss.str("");
	cout << result_print_all_i << "\n";
	auto verifyResultAndClear_print_all = [&result_print_all_i](stringstream& ss) { assert( result_print_all_i + ", " == ss.str() || result_print_all_i == ss.str() ); ss.str(""); };
	cout << "print_all_[ii, v]: ";
	//	{{{
	print_all_ii(ss, 1, 2, 3, 8, 7, "53");
	verifyResultAndClear_print_all(ss);
	print_all_iii(ss, 1, 2, 3, 8, 7, "53");
	verifyResultAndClear_print_all(ss);
	print_all_iv(ss, 1, 2, 3, 8, 7, "53");
	verifyResultAndClear_print_all(ss);
	print_all_v(ss, 1, 2, 3, 8, 7, "53");
	verifyResultAndClear_print_all(ss);
	//	}}}
	cout << "verified all\n";
	cout << "\n";

	//	Ongoing: 2022-03-20T01:15:17AEDT (uncomment after changing implementation such that 'fwd_v_.*' (functions) no longer print umpteen-million lines)
	//	{{{
	////	'fwd_v_.*' Usage:
	//cout << "fwd_v:\n";
	//fwd_v_i(5, "abc", 3.0, Widget());
	//cout << "\n";
	//fwd_v_ii(5, "abc", 3.0, Widget());
	//cout << "\n";
	//	}}}

	//	Ongoing: 2022-03-18T21:38:35AEDT 'print_details' [...] but it correctly prints the type given to it as well (as opposed to current verison, where types become references)
	//	'my_make_unique<T>()' Usage:
	cout << "my_make_unique:\n";
	auto pw1 = my_make_unique<Widget>();
	//my_make_unique<vector<int>>( {1,2,3} ); 	//	error, literal {} is perfect-forwarding failure case (see item 30)
	auto ps1 = my_make_unique<vector<int>>( initializer_list<int>({1,2,3,4}) );	
	cout << get_type_name<decltype(pw1)>() << "\n";
	cout << get_type_name<decltype(ps1)>() << "\n";
	cout << "\n";

	//	get_types_names<Ts...> Usage:
	cout << "get_types_names:\n";
	auto gtn_v1 = get_types_names<string,string,int,long,double,Widget,bool>();
	print_vector_elements(cout, gtn_v1);
	cout << "\n";

	//	Variadic class template can be instantiated with any number of template arguments
	//cout << "Tuple:\n";
	//Tuple<> t0;
	//Tuple<int> t1;
	//Tuple<int,float> t2;
	////Tuple<0> t3;			//	error, '0' is not a type
	//cout << "\n";


	//	Ongoing: 2022-03-20T01:17:14AEDT 'foo' -> not a helpful example?
	//	{{{
	//	'foo(Types...)' Variadic function template can be called with any number of arguments
	//foo();				//	foo<>
	//foo(1);				//	foo<int>
	//foo(2, 1.0);		//	foo<int,double>
	//	}}}
	//(void) t0; (void) t1; (void) t2;
	return 0;
}

