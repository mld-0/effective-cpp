#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	TODO: 2022-01-30T16:39:35AEDT effective-c++, extras/specialize-templates.cpp, (what does it mean) (and how might one be expected to remember said explanation) (need a better explanation?) (does either effective-c++ book touch the topic?)

//	LINK: http://www.gotw.ca/publications/mill17.htm

//	class template:
template<class T> class X {};					//	(a)

//	function template with two overloads:
template<class T> void f(T) {}					//	(b)
template<class T> void f(int, T, double) {}		//	(c)

//	These unspecialised templates are called the underlying base templates
//	These can be specialized
//		A class template can be partially specialized and/or fully specialized
//		A function template can only be fully specialized (but can also be overloaded)

//	A partial specialization of (a) for pointer types:
template<class T> class X<T*> {};

//	A full specialization of (a) for int:
template<> class X<int> {};

//	A separate base template that overloads (b) and (c)
//	Not a partial specialization (there is no such thing as a partial specialization of a function template)
template<class T> void f(T*) {}					//	(d)
//	a full specialization of (b) for int
template<> void f<int>(int) {}					//	(e)

//	A plain function, that happens to overload (b), (c), and (d), but not (e)
void f(double) {}								//	(f)

//	Non-template functions are preferred over template functions when selecting a function that matches parameter types
//	If no matching non-template function is found, the best matching function template is selected <(If it's clear that there's one "most specialized" function base template, that one gets used. If that base template happens to be specialized for the types being used, the specialization will get used, otherwise the base template instantiated with the correct types will be used. Else if there's a tie for the "most specialized" function base template, the call is ambiguous because the compiler can't decide which is a better match. The programmer will have to do something to qualify the call and say which one is wanted.)>


//	Another example:
template<class T>		//	(a)		A base template
void g(T) {}

template<class T>		//	(b)		A second base template, overloads (a) 
void g(T*) {}			//			(function templates can't be partially specialized, they overload instead)

template<>				//	(c)		explicit specialization of (b)
void f<>(int*) {}


int main()
{
	bool b;
	int i;
	double d;

	//	From example 1:
	f(b);				//	Calls (b) with T=bool
	f(i, 42, d);		//	Calls (c) with T=int
	f(&i);				//	Calls (d) with T=int
	f(i);				//	Calls e
	f(d);				//	Calls (f)


	int *p;

	//	From example 2:
	f(p);				//	calls (b) 
						//	(overload resolution ignores specializations and operates only on base function templates)
						//	since specializations don't overload

	
	//	Morals:
	//	1) If you want to customize a function base template, and want that customization to participate in overload resolution (or to always be used in the case of an exact match), make it a plain function, not a specialization <(and provide overloads, avoid providing specializations)>
	//	2)	If you're writing a function base template, prefer to write it as a single function template that should never be specialized or overloaded, and then implement the function template entirely as a simple handoff to a class template containing a static function with the same signature (which everyone can specialize - fully and partially - without affecting the results of overload resolution)


	return 0;
}

//	Illustrating moral 2:
template<class T>
struct FImpl;

template<class T>
void h(T t) { FImpl<T>::f(t); } 	//	users don't touch this

template<class T>
struct FImpl {
	static void h(T t); 			//	users, specialize this
};


//	Summary:
//		It is ok to overload function templates, overload resolution considers all base templates and picks the best match

//		It is a lot less intuitive to specialize function templates: they cannot be partially specialized (the language prohibits it), and they do not overload. This means any specializations that have been written do not affect which template gets used (which is counter-intuitive). If one writes a nontemplate function with identical signature instead of a function template specialization, the nontemplate is selected by default.
//		If you're writing a function template, prefer to write it as a single function template that should never be specialized or overloaded, and implement the function template entirely in terms of a class template.
//		If you're using someone else's plain old function template (one that's not implemented in terms of a class template as suggested above), and you want to write your own special-case version that should participate in overloading, don't make it a specialization; just make it an overloaded function with the same signature.

