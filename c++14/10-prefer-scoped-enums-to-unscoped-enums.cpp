//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	Use scoped (class) enums instead of unscoped enums. 

//	TODO: 2022-03-20T23:17:08AEDT effective-c++, item 10, prefer-scoped-enums, implement 'primeFactors'
vector<int> primeFactors(int x) {
	return {};
}

void unscoped_enum() {
	//	<C++98> unscoped enum:
	//	'black', 'white', and 'red' are declared in the same scope as 'Color'
	enum Color { black, white, red, };
	//auto white = false;						//	error, redefinition of 'white'
	Color c1 = white;
	auto c2 = white;
	//	Ongoing: 2022-03-20T23:20:06AEDT warning about comparison of unscoped enum value with double from YCM (that is is deprecated), but not from compiler(?) (also no warning for use of 'primeFactors(c1)') (and no warning for comparison with integer other than 'always false' for sufficently large value)
	//	Unscoped enums implicitly convert to numeric types.
	//	This means they can be used in contexts in which they are meaningless:
	if (c1 > 3) {
		auto pf_c1 = primeFactors(c1);	
	}
	//if (c1 > 53.6) {}		//	YCM warning (but no compiler warning), comparison with double deprecated
	(void) c1; (void) c2;
}

void scoped_enum() {
	//	<C++11> scoped enum:
	//	Does not leak names in the same way
	enum class Color { black, white, red, };
	auto white = false;
	//Color c = white;							//	error, no enumerator named 'white' in this scope
	Color c1 = Color::white;
	auto c2 = Color::white;
	//	Scoped enums are much more strongly typed than unscoped enums. There is no implicit conversion to any other type.
	//if (c1 > 3) {}							//	error, invalid operands
	//auto pf_c1 = primeFactors(c1);			//	error, no matching function
	//	It is legal (but meaningless) to cast scoped enum values to numeric types
	if (static_cast<double>(c1) > 3) {
		auto pf_c2 = primeFactors(static_cast<int>(c1));
	}
	(void) white; (void) c1; (void) c2;
}

//	When declaring an enum (scoped or unscoped), assigning quantities to each value is optional.
enum Status_ii { good = 0, failed = 1, incomplete = 100, corrupt = 200, audited = 500, indeterminate = 0xFFFFFFFF, };

//	A scoped enum uses 'int' unless otherwise specified. 

//	error, 0xFFFFFFFF cannot be narrowed to type 'int'
//enum class Status_iii { good = 0, failed = 1, incomplete = 100, corrupt = 200, audited = 500, indeterminate = 0xFFFFFFFF, };		

//	Unscoped enums cannot be forward declared without a type.
//enum Status;									//	error

//	Ongoing: 2022-03-21T17:32:43AEDT consistency requirements on the use of type when forward declaring scoped enums
//	Scoped enums can be forward declared without a type.
enum class Status;
//	Forward declaration is sufficent to use enum values as function parameters. 
void continueProcessing(Status s) {}

//	All enums have an underlying type. In C, this is int. For unscoped enums, this is determined by the compiler. Unless specified otherwise, scoped enums are int. 
//	The type of an enum can be overruled:
enum Color_us_long: long { black, white, red, };
enum class Color_s_long: long { black, white, red };

//	Unscoped enums can be forward declared provided their type is provided.
enum Status_iv: int;

//	Ongoing: 2022-03-21T17:38:14AEDT list (braced) initalization


//	Ongoing: 2022-03-21T18:44:38AEDT I will not believe indexing a tuple is an acceptable use for an enum
//	Ongoing: 2022-03-21T17:59:57AEDT what type is the most correct for (use in) 'get<>'? (is it size_t?)
//	Ongoing: 2022-03-21T17:57:19AEDT we are relying on the default values being 0,1,2,...
//	Example: unscoped vs scoped enums for naming tuple indexes
using UserInfo = tuple<string,string,int>;
UserInfo u1 = { "username", "email@address.com", 53 };
enum UserInfoFields_us { name, email, reputation };
auto u1_name_i = get<name>(u1);
//	The strong type safety of scoped enums makes their use for this purpouse awkward:
enum class UserInfoFields_s { name, email, reputation };
auto u1_name_ii = get<static_cast<int>(UserInfoFields_s::name)>(u1);


//	Example: convert enum values to underlying type
template<typename T>
constexpr auto toTupleIndex(T enumerator) noexcept {
	return static_cast<std::underlying_type_t<T>>(enumerator);
}
auto u1_name_iii = get<toTupleIndex(name)>(u1);


int main()
{
	return 0;
}

//	Summary:
//		<C++98> style enums are known as 'unscoped enums'. <New-C++> introduceds 'scoped enums' which should be prefered.
//		Scoped Enums are declared as 'enum class VarName { <...> };'
//		Enums have an underlying type. For unscoped enums, if not specified, this is determined by the compiler. Scoped enums have a default underlying type of 'int'. 
//		Scoped enums may always be forward declared. Unscoped enums may only be forward declared with a custom underlying type.

