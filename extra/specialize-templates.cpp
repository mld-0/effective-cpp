//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{1

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
template<> 
void custom_sort<char>(char arr[], int size) {
	cerr << "custom_sort() counting sort\n";
	//	TODO: 2022-01-30T23:57:49AEDT effective-c++, extras/specialize-templates.cpp, (for completeness (and elegance) sake), implemented the omitted (hand-waved) (in-place) counting sort for 'void custom_sort<char>(char arr[], int size)' 
	//	implement counting sort
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


//	Partial template specialiation, Supported By:
//		class templates
//		variable templates (since c++14)
//		<(not function templates (a restriction for what c++ versions?))>
//		<...>

//	<(means defining only some of the template type variables?)>
//	<details>



void TemplateSpecializationIntro() {
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
