//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	TODO: 2022-04-29T03:32:36AEST effective-c++, extra/member-variable-initialization, member variables that are static and/or const
//	TODO: 2022-04-29T03:42:59AEST effective-c++, extra/member-variable-initialization, constructor delegation

//	Initialing a variable:
int value1 = 1; 		// copy initialization
double value2(2.2); 	// direct initialization
char value3 {'c'}; 		// uniform initialization

//	Class member variables in C++ are initalized in the order they are declared in the class (and not in the order they appear in the member initialization list - the member initialization list may not be executed in the order it is written (best practice is to write it in the order it will be executed)).
//	<(If a member variable is assigned a value both in its definition, and in the ctor, the value in the ctor is used and the other value is ignored(?))>

//	<(Use of brace-or-equal-initalizer for member inital values saves clutter in the ctor <definition?>)>
//	<((best-practice is to) provide inital values in the declaration wherever possible (and only in the ctor (and then in the initialization list) if the value must be chosen by the ctor) (does 'constructor delegation' come into best-practice?))>
//	{{{
//	Ongoing: 2022-04-29T03:48:02AEST constructor delegation, (is it <default/implicit> for a custom ctor to call the default ctor? presumedly not since the existance of a custom ctor precludes the implicit default ctor?)
//	Ongoing: 2022-04-29T02:36:47AEST ctor member initialization lists belong in the declaration not definition(?)
//	Ongoing: 2022-04-29T02:41:46AEST any member variable declared and not given a value in the declaration/ctor is undefined(?) (aren't objects default constructed?) (or can one create classes of uninitalized values (and might one assume one shouldn't <do/allow> that?))
//	Ongoing: 2022-04-29T02:43:16AEST (should also address the question (here)) - equals initialization vs braced initialization (vs initalizer list initialization) (vs ?)
//	Ongoing: 2022-04-29T03:21:46AEST are the rules for equals/parenthesis/braced initialization any different for initalizing class member variables?
//	Ongoing: 2022-04-29T03:18:30AEST (can one presume) ctor default arguments are a bad way to provide default values for member variables? (worse than ctor delegation?)
//	Ongoing: 2022-04-29T03:13:51AEST constructor delegation (something not to do because one should just provide values in the class declaration?) (a mess of ctors calling ctors (to set default values?))
//	}}}

//	<(If class members are neither mentioned in a constructor's member initializer list nor have a brace-or-equal-initializer, then they get default-initialized. That means, that for class types the default constructor is called, but for any other types like enums or built in types like int, double, pointers, no initialization happens at all. This applies for each element of array, and, as a corollary, it applies for plain old data classes as well, as their default constructor in turn default-initializes all of their members. No initialization means your member variables possibly contain garbage values.)>

//	Providing brace-or-equal-initializers makes a class nontrivial. (a nontrivial class is a class with a nontrivial dtor?)
//	Ongoing: 2022-04-29T02:46:01AEST (is this not true) (making a class non-trivial) for providing values in the initialization list / ctor (as opposed to braced/equals initialization (in the declaration) (which does?))
//	Ongoing: 2022-04-29T03:35:08AEST <disadvantages/downsides> to a class being non-trivial? (which it becomes if we initalize member variables in their declaration?)

//	LINK: https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/
//	{{{
//	Formatting initalizer lists:
//		Everything on one line:
//		Something() : m_value1{ 1 }, m_value2{ 2.2 }, m_value3{ 'c' } 

//	Indented on next line:
//	    Something(int value1, double value2, char value3='c') 
//	        : m_value1{ value1 }, m_value2{ value2 }, m_value3{ value3 } 

//	One per line:
//    Something(int value1, double value2, char value3='c', float value4=34.6f) 
//        : m_value1{ value1 } 
//        , m_value2{ value2 }
//        , m_value3{ value3 }
//        , m_value4{ value4 }

//	}}}

//	LINK: https://stackoverflow.com/questions/11594846/default-member-values-best-practice
//	{{{
//	If a class member is always initialized with the same initial value, then you should make the initializer inline ... (what are you even saying?) ... If the initial value depends on the constructor, then put it in the constructor initializer list, (And never use assignment in the way you did ((is) he is talking about declaring the value in the cpp file?) (or is he saying don't assign a value in the ctor body?)).

//	you should use constructors initialisation list 'Foo::Foo() : greet("hello") { }' - this avoids default initialisation + assignment in favour of direct initialisation by value. For complex types, this can make a significant difference, more important, though: Some types (references, constant members, non-default constructible ones) only can be initialised that way (in the constructor, apart from assigning a default).

//	It depends whether you need to stay compatible with older C++ compilers When you are not using (using older than) <C++11> you have to initialize most members (all non-static) in the constructor.

//	Note that providing an inline initializer prevents your class from having a trivial default constructor.

//	Ongoing: 2022-04-29T02:26:02AEST is an 'inline initalizer' where a member variable is declared and assigned a (default) value in the same statement?
//	Ongoing: 2022-04-29T02:27:25AEST there appears to be an argument about whether such inital values belong in the h/cpp file(?)

//	Further many people advocate to explicitly initialize every member even if this means explicitly calling the default ctor: <(but why though? are not member variables default constructed?)>

//	}}}

//	Ongoing: 2022-04-29T02:28:20AEST (see below) God's opinion on the subject? (and since he offers an FAQ, his opinion on many other subjects?)
//	LINK: https://www.stroustrup.com/C++11FAQ.html#member-init
//	{{{
//	<C++98>: only static const intergal types can be initalized in class (and only with <constant expressions>
//	<C++11>: non-static data members can be initalized in their declaration in the class
//	}}}

//	LINK: https://www.learncpp.com/cpp-tutorial/default-member-initialization/
//	{{{
//	}}}

//	LINK: https://en.cppreference.com/w/cpp/language/default_initialization
//	{{{
//	}}}

//	LINK: https://www.sandordargo.com/blog/2020/01/08/default-member-initialization
//	{{{
//	Suggestion: explicitly initalize every member variable (even if that means calling the default ctor) (even though, (as per the C++ specification), (anything that can be default constructed will be (by default))
//	Core Guideline: Don't define a default-ctor that only initalizes data members, use in-class member initalizers instead
//	}}}

//	LINK: https://stackoverflow.com/questions/12170160/c-initialization-of-member-variables
//	{{{
//	}}}

//	LINK: https://stackoverflow.com/questions/13961037/delegate-constructor-c
//	LINK: https://www.geeksforgeeks.org/constructor-delegation-c/
//	{{{
//	}}}

//	Example: initalizing an array member variable
//	<>

//	Example: initalize an object (custom type) member variable
//	<>

int main()
{
	return 0;
}

