//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
using namespace std;
//	{{{2
//	get_vector_randoms(), get_type_name<T>()
//	{{{
vector<int> get_vector_randoms(int size, int min=0, int max=9) {
//	{{{
	//	values generated are in range [min, max] 
    std::uniform_int_distribution<int> distribution(min, max);
    std::default_random_engine generator;
	generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	vector<int> result(size);
	auto gen = [&distribution,&generator]() { return distribution(generator); };
	std::generate(result.begin(), result.end(), gen);
	return result;
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
//	}}}

//	Ongoing: 2022-03-27T18:30:40AEDT this is (not) the place for a discussion of what an iterator is(?)

//	Support for const-iterators in <C++98> was so poor their use was rarely considered practical.
//	<C++11> provides const-iterators that are easier to get and easier to use.

//	'const iterator' is an iterator that cannot be incremented <(can we modify the value to which it points)> -> <(not particuarlly meaningful)> <(it is effectively a pointer?)>
//	'const_iterator' is an iterator whose values cannot be modified

//	const_iterator does not convert into iterator (not even with static_cast/reinterpret_cast)
//	iterator can be static_cast into const_iterator

//	Ongoing: 2022-03-27T17:11:21AEDT <(on supporting const_iterators, implement functions <using/in-terms-of> const_iterators where possible, (the implication being) non-const iterators can be used as const_iterators)>

//	<C++11> provides nonmember versions of begin/end. <C++14> adds nonmember cbegin/cend. 
//	Maximally generic code should use these non-member versions of these functions.
//	These non-member functions also work on a primative array <((but) can they work with a pointer to an array -> a pointer does not contain length information, no way to get end?)>

//	Example: findAndInsert(), insert insertVal before targetVal
//	'find(first,last,val)' returns an iterator to first element [first,last) that matches val, or last if not found
//	'insert(position,val)' inserts new element before position
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal) {
	using std::cbegin;
	using std::cend;
	auto it = std::find(cbegin(container), cend(container), targetVal);
	container.insert(it, insertVal);
}


//	Example: When writing <C++11>, it may be necessary to manually implement cbegin/cend
//	The result of 'begin' for a reference-to-const container is a const_iterator <(both member and non-member versions?)>
//	This technique allows us to implement my_cbegin for containers which only support 'begin'.
template<class C>
auto my_cbegin(const C& container) -> decltype(std::begin(container)) {
	return std::begin(container);
}


int main()
{
	vector<int> v1 = get_vector_randoms(5);

	//	Each std::vector 'get iterator' function has a corresponding 'get const_iterator' function:
	//	'cbegin'/'cend' correspond to 'begin'/'end', 
	//	'crbegin'/'crend' correspond to 'rbegin'/'rend'

	//	deduced type: 'std::__wrap_iter<int *>'
	for (auto it = v1.begin(); it != v1.end(); ++it) {
		cout << *it << " ";
	}
	cout << "\n";
	cout << "\n";

	//	deduced type: 'std::__wrap_iter<const int *>'
	for (auto it = v1.cbegin(); it != v1.cend(); ++it) {
		cout << *it << " ";
	}
	cout << "\n";
	cout << "\n";

	//	a 'const iterator' is akin to a pointer, its value can be changed, but it cannot be incremented
	auto v2 = get_vector_randoms(10);
	const auto cit2 = v2.begin();
	*cit2 = 9;
	//++cit2;

	//	Ongoing: 2022-03-27T17:32:44AEDT std::find() returns a const_iterator(?)
	//	Example: using const_iterators with find/insert
	vector v3 = { 9, 5, 7, 22, 53, 89, 4 };
	auto v3_insert = std::find(v3.cbegin(), v3.cend(), 53);
	v3.insert(v3_insert, 1);
	for (auto it = v3.cbegin(); it != v3.cend(); ++it) {
		cout << *it << " ";
	}
	cout << "\n";
	cout << "\n";

	//	Example: cbegin/cend/find with primative array
	int a1[] = { 2, 3, 4, 7, 9, 12, 15, };
	auto pFind1 = std::find(cbegin(a1), cend(a1), 9);
	cout << *pFind1 << "\n";
	for (auto it = cbegin(a1); it != cend(a1); ++it) {
		cout << *it << " ";
	}
	cout << "\n";
	//	Cannot use cbegin/cend/find with pointer to primative array
	int *pa1 = a1;
	//auto pFind2 = std::find(cbegin(pa1), cend(pa1), 9);			//	error, no matching function cbegin/cend
	cout << "\n";

	////cout << "get_type_name<my_cbegin(v3)>():\n";
	//cout << get_type_name<decltype(my_cbegin(v3))>() << "\n";
	//cout << "\n";

	(void) pa1;
	return 0;
}

//	Summary:
//		<C++14> provides complete support for const_iterator
//		Prefer const_iterators to iterators
//		cbegin/cend correspond to begin/end
//		For maximally generic code, prefer non-member versions of begin/end/cbegin/ect.

