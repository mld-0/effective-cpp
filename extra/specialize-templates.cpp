//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	TODO: 2022-01-30T16:39:35AEDT effective-c++, extras/specialize-templates.cpp, (what does it mean) (and how might one be expected to remember said explanation) (need a better explanation?) (does either effective-c++ book touch the topic?)

//	Template Specialization:
//	A template defines a group of classes or a group of functions. Template specialization allows special behaviour for that class to be defined for particular data types

//	Ongoing: 2022-01-31T00:02:03AEDT this (first example 'custom_sort()') is <partial/complete> <implicit/explicit> template specialization?

//	For example, define a 'custom_sort()' function template, with special (counting sort) behaviour for 'char' type
template<typename T>
void custom_sort(T arr[], int len) {
	cerr << "custom_sort() for typename T\n";
	stable_sort(arr, arr+len);
}
//	LINK: https://www.geeksforgeeks.org/counting-sort/
template<> 
void custom_sort<char>(char arr[], int size) {
	cerr << "custom_sort() counting sort\n";
	char output[size];
	int RANGE = 256;
	//	array to store count of indervidual characters
	int count[RANGE];
	memset(count, 0, sizeof(count));
	//	Store count of each character
	for (int i = 0; arr[i]; ++i) {
		++count[arr[i]];
	}
	//	Set count[i] to the position of <i> in output array
	for (int i = 1; i <= RANGE; ++i) {
		count[i] += count[i-1];
	}
	//	Build output array
	for (int i = 0; arr[i]; ++i) {
		output[count[arr[i]]-1] = arr[i];
		--count[arr[i]];
	}
	//	Copy 'output' to 'arr'
	for (int i = 0; arr[i]; ++i) {
		cout << output[i] << "\n";
		arr[i] = output[i];
	}
}
//	The compiler checks for a <(partially/fully/best)> (matching) specialized version of a function, and (if found) it uses it instead of the main template

//	Ongoing: 2022-01-31T00:08:08AEDT (clarify) what is a 'variable template'?
//	LINK: https://en.cppreference.com/w/cpp/language/template_specialization
//	Explicit (full) template specialization, Supported by:
//		function template, 
//		class template, 
//		variable template, 
//		member function of class template, 
//		static data member of a class template, <meaning variable?>
//		member class of a class template
//		member enumeration of a class template
//		member class tempalte of a class or class template
//		member function template of a class or class template

//	<(means defining all of the template type variables?)>
//	<details>
//	An explicit specialization may be declared in any scope where its primary template may be defined. i
//	Specializations must appears after the non-specialized template declaration. 
//	Specializations must be declared before the first use that would cause implicit instantiation <(in every translation unit where such a use occurs)> (an implicit instantiation is a specialization of a template generated due to the use of that template)
//	A template specialization that has been declared but not defined can be used just like any other incomplete type (pointers and references to it may be used)
//	The explicit specialization of a function template is not necessary inline just because the primary template is 
//	Default function arguments cannot be specified in explicit specializations of function templates, member function templates, and member functions of class templates when the class is implicitly instantiated
//	An explicit specialization cannot be declared a friend

//	Ongoing: 2022-01-31T23:39:39AEDT (this <technique> is called what?) (using structs (instead of functions) why?)
template<typename T>
struct my_is_void: std::false_type {};
template<>
struct my_is_void<void> : std::true_type {};

namespace N {
	template<typename T> class X {};		//	primary template
	template<> class X<int> {};				//	specialization in same namespace 

	template<typename T> class Y {};		//	another primary template
	template<> class Y<double>;				//	forward declaration of specialization (in same namespace)
}
template<> class N::Y<double> {};			//	<(specialization in same namespace)>

//	When specializing a function template, its template arguments can be omitted if template argument deducation can provide them from the function arguments:
template<class T> class Array {};
template<class T> void sort(Array<T>& v);		//	primary template
template<> void sort(Array<int>& v);			//	No need to write 'template<> void sort<int>(Array<int>& v)'

//	When defining a member of explicitly specialized class template outside the body of the class, the syntax 'template <>' is not used, unless if its a member of an explicitly specalizd member class template
//	{{{
template< typename T>
struct A
{
    struct B {};                    // member class 
    template<class U> struct C { }; // member class template
};

template<> // specialization
struct A<int> 
{
    void f(int); // member function of a specialization
};

// template<> not used for a member of a specialization
void A<int>::f(int) { /* ... */ }
template<> // specialization of a member class
struct A<char>::B
{
    void f();
};

// template<> not used for a member of a specialized member class either
void A<char>::B::f() { /* ... */ }

template<> // specialization of a member class template
template<class U> struct A<char>::C
{
    void f();
};

// template<> is used when defining a member of an explicitly
// specialized member class template specialized as a class template
template<>
template<class U> void A<char>::C<U>::f() { /* ... */ }
//	}}}

//	An explicit specialization of a static data member of a template if a definition if the declaration includes an initalizer, otherwise it is a declaration
//template<> X Q<int>::x;		//	declaration of a static member
//template<> X Q<int>::x ();	//	error, function declaration
//template<> X Q<int>::x {};	//	definition of a default-initialized static member

//	Templates may be nested within many enclosing class templates. In an explicit specialization for such a member, there is a 'template<>' for each enclosing class template that is explicit specified
template<class T1> struct Outer {
	template<class T2> struct Inner {
		template<class T3> void mf();
	};
};
template<> struct A<int>;
template<> template<> struct Outer<char>::Inner<double>;
template<> template<> template<> void Outer<char>::Inner<char>::mf<double>();
//	In such a nested declaration, some of the levels may remain unspecified <(except that it can't specialize a class member template if its enclosing class is unspecified)>
//	For each <such level>, the declaration needs 'template<arguments>' because such specializations are themselves templates:
//	{{{
template <class T1> class AA
{
    template<class T2> class BB
    {
        template<class T3> void mf1(T3); // member template
        void mf2();                      // non-template member
    };
};

// specialization
template<> // for the specialized A
template<class X> // for the unspecialized B
class AA<int>::BB
{
    template <class T> void mf1(T);
};

// specialization
template<> // for the specialized AA
template<> // for the specialized BB
template<class T> // for the unspecialized mf1
void AA<int>::BB<double>::mf1(T t) {}

// ERROR: B<double> is specialized and is a member template, so its enclosing A
// must be specialized also
//template<class Y>
//template<> void AA<Y>::BB<double>::mf2() {}
//	}}}

//	Partial template specialiation, Supported By:
//		class templates
//		variable templates (since c++14)
//		<(not function templates (a restriction for what c++ versions?))>
//		<...>

//	<(means defining only some of the template type variables?)>
//	<details>




void TemplateSpecializationIntro() {
	cout << "my_is_void<char>=(" << my_is_void<char>::value << ")\n";
	cout << "my_is_void<void>=(" << my_is_void<void>::value << ")\n";
	cout << "\n";

	//	Ongoing: 2022-02-07T22:21:34AEDT (do we not) (ever) call (it as) custom_sort<char>()?

	//	custom_sort (int):
	cout << "custom_sort<int>:\n";
	int A[] = { 3, 7, 9, 4, 8, 5, 2, };
	for (const auto& x: A) { cout << x << " "; } cout << "\n";
	custom_sort(A, sizeof(A)/sizeof(*A));
	for (const auto& x: A) { cout << x << " "; } cout << "\n";
	cout << "\n";

	//	custom_sort (char):
	cout << "custom_sort<char>:\n";
	char B[] = { 3, 7, 9, 4, 8, 5, 2, };
	for (const auto& x: B) { cout << (int) x << " "; } cout << "\n";
	custom_sort(B, sizeof(B)/sizeof(*B));
	for (const auto& x: B) { cout << (int) x << " "; } cout << "\n";
	cout << "\n";

}




//	TODO: 2022-01-30T22:35:44AEDT effective-c++, extra/specializer-templates.cpp, laid-out-rules (in paragraph or points) that predict/explain the selection made for each call to 'f()' and 'g()', (and better laid out examples?) (examples reflecting a newer C++ than pre-01)
//	Why Not Specalize Function Templates:
//	{{{
//	LINK: http://www.gotw.ca/publications/mill17.htm
//	class template:
template<class T> class X {};					//	(1.a)

//	function template with two overloads:
template<class T> void f(T) { cout << "1.b\n"; }
template<class T> void f(int, T, double) { cout << "1.c\n"; }

//	These unspecialised templates are called the underlying base templates
//	These can be specialized
//		A class template can be partially specialized and/or fully specialized
//		A function template can only be fully specialized (but can also be overloaded)

//	A partial specialization of (1.a) for pointer types:
template<class T> class X<T*> {};

//	A full specialization of (1.a) for int:
template<> class X<int> {};

//	A separate base template that overloads (1.b) and (1.c)
//	Not a partial specialization (there is no such thing as a partial specialization of a function template)
template<class T> void f(T*) { cout << "1.d\n"; }	
//	a full specialization of (b) for int
template<> void f<int>(int) { cout << "1.e\n"; }

//	A plain function, that happens to overload (1.b), (1.c), and (1.d), but not (1.e)
void f(double) { cout << "1.f\n"; }

//	Non-template functions are preferred over template functions when selecting a function that matches parameter types
//	If no matching non-template function is found, the best matching function template is selected <(If it's clear that there's one "most specialized" function base template, that one gets used. If that base template happens to be specialized for the types being used, the specialization will get used, otherwise the base template instantiated with the correct types will be used. Else if there's a tie for the "most specialized" function base template, the call is ambiguous because the compiler can't decide which is a better match. The programmer will have to do something to qualify the call and say which one is wanted.)>

//	Another example:
template<class T>					//	A base template
void g(T) { cout << "2.a\n"; }

template<class T>					//	A second base template, overloads (a) 
void g(T*) { cout << "2.b\n"; }		//	(function templates can't be partially specialized, they overload instead)

template<>							//	explicit specialization of (b)
void g<>(int*) { cout << "2.c\n"; }

void WhyNotSpecializeFunctionTemplates()
{
	bool b = true;
	int i = 4;
	double d = 7.3;

	//	From example 1:
	f(b);				//	Calls (1.b) with T=bool		template<class T> void f(T);
	f(i, 42, d);		//	Calls (1.c) with T=int		template<class T> void f(int, T, double);


	//	Ongoing: 2022-01-30T22:30:24AEDT (1.d) f(&i) call produces no output
	f(&i);				//	Calls (1.d) with T=int		template<class T> void f(T*);


	f(i);				//	Calls (1.e) 				template<> void f<int>(int);
	f(d);				//	Calls (1.f) 				void f(double);

	int *p = &i;

	//	From example 2:
	//	Ongoing: 2022-01-30T22:34:29AEDT (according to link), should call 2.b (appears to call 2.c -> are there platforms where 2.b is called?) <(why are the answers wrong?)>
	g(p);				//	calls (2.c) ~~(b)~~
						//	(overload resolution ignores specializations and operates only on base function templates)
						//	since specializations don't overload

	//	Ongoing: 2022-01-30T22:39:13AEDT (presumedly someone explains this too better)
	//	Morals:
	//	1) If you want to customize a function base template, and want that customization to participate in overload resolution (or to always be used in the case of an exact match), make it a plain function, not a specialization <(and provide overloads, avoid providing specializations)>
	//	2)	If you're writing a function base template, prefer to write it as a single function template that should never be specialized or overloaded, and then implement the function template entirely as a simple handoff to a class template containing a static function with the same signature (which everyone can specialize - fully and partially - without affecting the results of overload resolution)

}

//	Ongoing: 2022-01-30T22:39:49AEDT (moral 2) what/where/why?
//	Illustrating moral 2: 
template<class T>
struct FImpl;

template<class T>
void h(T t) { FImpl<T>::f(t); } 	//	users don't touch this

template<class T>
struct FImpl {
	static void h(T t); 			//	users, specialize this
};

//	(Why Not Specalize Function Templates) Summary:
//		It is ok to overload function templates, overload resolution considers all base templates and picks the best match
//		It is a lot less intuitive to specialize function templates: they cannot be partially specialized (the language prohibits it), and they do not overload. This means any specializations that have been written do not affect which template gets used (which is counter-intuitive). If one writes a nontemplate function with identical signature instead of a function template specialization, the nontemplate is selected by default.
//		If you're writing a function template, prefer to write it as a single function template that should never be specialized or overloaded, and implement the function template entirely in terms of a class template.
//		If you're using someone else's plain old function template (one that's not implemented in terms of a class template as suggested above), and you want to write your own special-case version that should participate in overloading, don't make it a specialization; just make it an overloaded function with the same signature.
//	}}}


int main()
{
	cout << "TemplateSpecializationIntro():\n";
	TemplateSpecializationIntro();
	cout << "\n";

	cout << "WhyNotSpecializeFunctionTemplates():\n";
	WhyNotSpecializeFunctionTemplates();
	cout << "\n";

	return 0;
}
