//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	Ongoings:
//	{{{
//	Ongoing: 2022-05-12T04:23:32AEST copy-elision, when passing by-value vs by-const-value(?)
//	Ongoing: 2022-05-10T01:55:21AEST which item/example mentions fast passing by-value?
//	Ongoing: 2022-05-10T01:55:48AEST (verify) fast passing by-value is being used?
//	Ongoing: 2022-05-11T03:10:40AEST (for each C++ version), (when is copy-elision required?)
//	Ongoing: 2022-05-11T03:17:37AEST (as of <C++17>, copy-elision is required when-returning/always?)
//	}}}

//	<C++?> idiom: 'Want speed? Pass by value'
//	LINK: https://web.archive.org/web/20140205194657/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
//	LINK: https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization

//	copy-elision: omit copy/move ctors, and their side effects, resulting in zero-copy pass-by-value semantics
//	LINK: https://en.cppreference.com/w/cpp/language/copy_elision

//	Fastest, call by value/reference/pointer: (only way to tell is to profile code)

//	epsilon: <(not applicable to 'fast by-value', (a thing for) minimum floating point error?)>


//	return-value-optimisation:




//	LINK: https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization
//	{{{
//	copy-elision: an optimization done by most compilers, to prevent extra copies in certain situation.
//	Makes pass/return by value feasible in practice.
//	<Only?> C++ optimization that violates the as-if rule: copy-elision can be performed even if copying/moving has side effects.
//	Because of copy-elision, one cannot rely on a specific number of ctor/dtor calls - one should not have critical logic inside copy/move-ctors or dtors.
//	<C++17> onwards, copy-elision is <guaranteed/required> <(always/when-returning?)> <(even with a deleted copy-ctor?)>


//	Example: 'Foo f = Foo()', copy-ctor may be called 2x, 1x, or 0x times, all of which are valid C++
struct Foo {
	Foo() {};
	Foo(const Foo&) { cout << "Foo(const Foo&)\n"; }
};
Foo make_Foo() { return Foo(); }
void example_multiple_valid_outputs() {
	cout << "example_multiple_valid_outputs\n";
	Foo f = Foo();
}
//	possible valid outputs:
//	{{{
//		example_multiple_valid_outputs
//		Foo(const Foo&)
//		Foo(const Foo&)
//	or
//		example_multiple_valid_outputs
//		Foo(const Foo&)
//	or
//		example_multiple_valid_outputs
//	}}}
//	As of <C++17>, Copy Elision is guaranteed when an object is returned directly
//	{{{
//	struct C {
//	  C() {}
//	  C(const C&) { std::cout << "A copy was made.\n"; }
//	};
//	 
//	C f() {
//	  return C(); //Definitely performs copy elision
//	}
//	C g() {
//	    C c;
//	    return c; //Maybe performs copy elision
//	}
//	}}}


//	Common Forms of copy-elision
//		Named return value optimization: when a named object (variable) is returned by value (into caller variable)
//				Widget w; 
//				return w;
//		Regular return value optimization: when a temporary thing <(rvalue?)> is returned (into caller variable)
//				return Widget()
//		Construction from temporary (into function parameter variable)
//				foo(Widget())
//		Throwing/catching exception by-value
//				catch (Thing c)

//	Common limitations of copy-elision
//	LINK: https://docs.microsoft.com/en-us/previous-versions/ms364057(v=vs.80)?redirectedfrom=MSDN#nrvo_cpp05_topic3
//		multiple return points
//		conditional initializations

//	Most commercial-grade compilers support copy elision & (N)RVO (depending on optimization settings). C++17 makes many of the above classes of copy elision mandatory <which?>.


//	Standard reference:
//	{{{
//	When certain criteria are met, an implementation is allowed to omit the copy/move construction of a class object, even if the copy/move constructor and/or destructor for the object have side effects. In such cases, the implementation treats the source and target of the omitted copy/move operation as simply two different ways of referring to the same object, and the destruction of that object occurs at the later of the times when the two objects would have been destroyed without the optimization. (Because only one object is destroyed instead of two, and one copy/move constructor is not executed, there is still one object destroyed for each one constructed). This elision of copy/move operations, called copy elision, is permitted in the following circumstances (which may be combined to eliminate multiple copies):
//	in a return statement in a function with a class return type, when the expression is the name of a non-volatile automatic object (other than a function or catch-clause parameter) with the same cv-unqualified type as the function return type, the copy/move operation can be omitted by constructing the automatic object directly into the function’s return value
//	in a throw-expression, when the operand is the name of a non-volatile automatic object (other than a function or catch-clause parameter) whose scope does not extend beyond the end of the innermost enclosing try-block (if there is one), the copy/move operation from the operand to the exception object (15.1) can be omitted by constructing the automatic object directly into the exception object
//	when a temporary class object that has not been bound to a reference (12.2) would be copied/moved to a class object with the same cv-unqualified type, the copy/move operation can be omitted by constructing the temporary object directly into the target of the omitted copy/move
//	when the exception-declaration of an exception handler (Clause 15) declares an object of the same type (except for cv-qualification) as the exception object (15.1), the copy/move operation can be omitted by treating the exception-declaration as an alias for the exception object if the meaning of the program will be unchanged except for the execution of constructors and destructors for the object declared by the exception-declaration.
//	}}}

//	automatic object: <(a variable on the stack)>

//	Under certain conditions, the compiler is allowed to omit the copy/move-ctor, even if they have side effects. <(In such cases, the source/target are treated as two different ways of referring to the same object)>. (This means the number of objects that are constructed/destructed can differ depending on the implementation, however this is still always one object destroyed for each one constructed). Copy-elision is permitted in the following circumstances (which may be combined to eliminate multiple copies):
//		1)	In a return statement of a function with a class return type, when the expression is the name of a non-volatile automatic object (other than a function / catch-clause parameter), with the same cv-unqualified type as the function return type. <(Not a function with multiple return paths?)>. The copy/move is omitted by constructing the automatic object directly into the return value <(that is, the variable the function result is being assigned to)>.
//		2)	In a throw-expression, when the operand (variable) is the name of a non-volatile automatic object (other than a function / catch-clause parameter) (whose scope does nto extend beyond the end of the innermost enclosing try-block). The copy/move is omitted by constructing the automatic object directly into the exception object.
//		3)	When a temporary class object that has not been bound to a reference would be copied/moved to a class object with the same cv-unqualified type. The copy/move is omitted by constructing the temporary object directly into the target variable.
//		4)	When the exception-declaration of an exception handler declares an object of the same type (except for cv-qualification) as the exception object. The copy/move is omitted by treating the exception-declaration as an alias for the exception object (if the meaning of the program will be unchanged by doing so, except for ctor/dtor calls).

//	Example: two calls to copy-ctor can be elided
//			1)	<(copying 't1' into the temporary object for the return value of 'make_Thing()'?)>
//			2)	copying that temporary object into 't2'
//	Effectively, construction of 't1' can be viewed as directly initalizing 't2' (and that objects destruction occurs when t2 goes out of scope). 
//	<(If we were to add a move-ctor to 'Thing', we elide a move from 'make_Thing()' (an rvalue) to 't2'?)> <((but) move would not be applicable to 't1'?)>
struct Thing {
	Thing() = default;
	~Thing() = default;
	Thing(const Thing&) { cout << "Thing(const Thing&)\n"; }
};
Thing make_Thing() {
	Thing t1;
	return t1;
}
void example_make_Thing() {
	Thing t2 = make_Thing();
}

//	Ongoings:
//	{{{
//	Ongoing: 2022-05-12T04:20:56AEST 'Copy elision is one way to look at it; object elision or object fusion'?
//	Ongoing: 2022-05-12T04:20:10AEST <(Even when copy elision takes place and the copy-/move-constructor is not called, it must be present and accessible (as if no optimization happened at all), otherwise the program is ill-formed)>
//	Ongoing: 2022-05-12T04:17:18AEST it is called 'copy-elision' (even though both copies/moves can be elided?)
//	Ongoing: 2022-05-12T04:14:44AEST (were copy-elision not to take place) (and move is available) (when does a copy become a move) (when doing it with an rvalue?)
//	Ongoing: 2022-05-12T03:56:50AEST (for 'Thing' copy-ctor, alike) specify '= default' as implementation, but also provide a custom implementation (print "Thing(const Thing&)" when called?)
//	Ongoing: 2022-05-12T03:47:57AEST 'exception-declaration of an exception handler' -> (this means the object declared in the 'catch (exception e)' <block/parenthesis>'?
//	Ongoing: 2022-05-12T03:46:23AEST (do the following mean the same) 'object of the same type (except for cv-qualification)' vs 'object of the same cv-unqualified type'?
//	Ongoing: 2022-05-12T03:29:43AEST (see above) (which C++ version does 'standard reference' apply to) (and call it something better than 'standard reference')
//	Ongoing: 2022-05-12T03:26:51AEST (meaning of certain condition 3 (see above)) (example(s) for each condition?)
//	Ongoing: 2022-05-12T03:25:26AEST <(in C++, an 'object' is an instance of a variable, a 'class object' is (what you might traditionally call an object) (a non-primitive variable))>
//	Ongoing: 2022-05-12T01:53:52AEST define 'automatic object' (and add the definition of 00-terminology) [...] also 'catch-clause parameter' (the thing being caught?)
//	Ongoing: 2022-05-12T01:33:59AEST copy-elision, and if I make changes to an object passed by-value, then an actual copy must have been performed at some point since the original object is unchanged?
//	Ongoing: 2022-05-12T01:40:05AEST copy-elision <(is not a copy being turned into a move, it is a way of performing a copy more efficiently than the copy-ctor could have performed it?)>
//	}}}
//	}}}

//	LINK: https://news.ycombinator.com/item?id=1442756
//	{{{

//	Guideline: Don’t copy your function arguments. Instead, pass them by value and let the compiler do the copying.
//	Lesson: don't explicitly copy variable references, just get their values and the compiler will copy optimize for you
//	{{{
//	In fewer words, the argument is this
//	
//	  std::vector<std::string> get_names();
//	  std::vector<std::string> const names = get_names();
//	Passing by value causes a lot of under-the-hood moving and copying which is slow. but we will learn later why this is actually correct.
//	  get_names(std::vector<std::string>& out_param );
//	  std::vector<std::string> names;
//	  get_names( names );
//	Passing by reference causes the need for many extra lines of code to be written throughout the code base, no more constants, mutating variables, other crap no one ever told you about when learning about pointers.
//	The solution to both of these is to use "RValue expressions". RValues are expressions that create anonymous temporary objects.
//	
//	When defining variables, using RValues allows transferring ownership of, in this case a dynamically allocated string array (vector), from the source vector to the target vector.
//	
//	When using functions, returns are also anonymous temporary objects, so we transfer the resources from the return value to the target value in the same way as with variables.
//	
//	Oh wait, the compiler actually takes care of optimizing stuff for you, it's called Return Value Optimization (RVO) and it works like this:
//	
//	  std::vector<std::string> names = get_names();
//	Oh shit, isn't this what we wrote as a first example that's expensive slow? yeah well, apparently there's nothing to worry about. use this.
//	Do pass a function within a function, because then you're passing rvalues as parameters which is unicorn-level magic
//	
//	  std::vector<std::string> sorted_names2 = sorted( get_names() );
//	RVO optimizations aren't required by any standard, but "recent versions of every compiler I’ve tested do perform these optimizations today."
//	Don't pass a variable by reference and then make an explicit copy of its values - that defeats the whole purpose of what we are trying to talk to you about
//	}}}

//	(Response?) This is very wrong for so many reasons and misses probably the most important reasons to pass by copy...
//	{{{
//	The example of the sorted functions is striking - because the compiler should be able to produce the same quality of code from both cases - the fact that it doesn't suggests that the compiler is not that smart. The r-value, l-value distinction is, in this case, unimportant - because of const the l-value can not be modified, as long as you don't use const_cast or forget a volatile keyword etc. elsewhere the compiler can safely assume that it really will remain const. I'll have to play with this myself I guess cos I might be missing something here that looking at bytes would reveal... but it seems that your example should fail to show any difference on a good compiler.
//	Worse though is that the const-reference and explicit copy is done at all - what possible reason could there be? This is where you should use pass by copy /anyway/, before your argument about speed, because you quite explicitly need to work with a copy. If you chose a suitable example you would see the opposite... unless your compiler is smart enough to treat your redundant copy as if it was a const-reference, which, e.g. the MS C++ compiler does infact do.
//	The important point I think that was missed though is to always pass small things by value - throwing heap pointers around to be dereferenced is typically much more expensive than using the stack - your compiler might optimise it away for you, but especially if you aren't using const references/pointers it can be difficult. Imagine your compiler is not smart - passing a 4/8-byte value as a reference to a 4/8-byte value is just silly. This is probably the most common case - ints, floats, doubles and even small structs will go on the stack, or better.
//	{{{
//	Yes, but in an article on copy elision, it doesn't really make sense to talk about small objects because it really doesn't matter if they are copied or not. Pass them by value anyways.
//	What are you referring to about the sorted function - that he says that the compiler isn't smart enough to optimize the copy out of a function returning function argument? His argument makes sense to me here, the caller doesn't know anything about the internals of the function, so it has to allocate separate space for the return value and the function argument, leading to at least one copy. With inline functions or whole program analysis (or some type of link time optimization) it should be smart enough to do this.
//	}}}
//	}}}

//	Ongoing: 2022-05-12T04:35:40AEST link (see above) (archive of 'Want Speed, Pass by value', 'ycombinator') has more discussion
//	}}}

//	LINK: https://stackoverflow.com/questions/35506708/move-constructor-vs-copy-elision-which-one-gets-called
//	{{{
//	}}}

//	LINK: https://docs.microsoft.com/en-us/previous-versions/ms364057(v=vs.80)?redirectedfrom=MSDN#nrvo_cpp05_topic3
//	{{{
//	}}}

//	LINK: https://stackoverflow.com/questions/9444485/why-is-rvo-disallowed-when-returning-a-parameter
//	{{{
//	}}}

//	LINK: https://shaharmike.com/cpp/rvo/
//	{{{
//	}}}

//	LINK: https://en.cppreference.com/w/cpp/language/copy_elision
//	{{{
//	}}}

//	Example: copy-elision and a deleted copy-ctor
//	<>


//	Example: modifying a value that has been copy-elided
//	<>

//	Example: verify copy-elision is happening(?)
//	<>

int main()
{

	example_multiple_valid_outputs();
	cout << "\n";

	example_make_Thing();
	cout << "\n";

	return 0;
}

