//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
//	{{{2
//	Ongoings:
//	{{{
//	Ongoing: 2022-05-14T15:24:00AEST (is return-value-optimization just an application of copy-elision -> (or) is it something different?)
//	Ongoing: 2022-05-12T04:23:32AEST copy-elision, when passing by-value vs by-const-value(?)
//	Ongoing: 2022-05-10T01:55:21AEST which item/example mentions fast passing by-value?
//	Ongoing: 2022-05-10T01:55:48AEST (verify) fast passing by-value is being used?
//	Ongoing: 2022-05-11T03:10:40AEST (for each C++ version), (when is copy-elision required?)
//	Ongoing: 2022-05-11T03:17:37AEST (as of <C++17>, copy-elision is required when-returning/always?)
//	}}}
//	TODO: 2022-05-14T15:09:50AEST effective-c++, extra/fast-by-value, (why it is better to pass iterators / function-objects by value?)
//	TODO: 2022-05-14T16:48:22AEST effective-c++, extra/by-ref-vs-val-swap-assignment (review example (and 'MyVector'))
//	TODO: 2022-05-14T17:10:34AEST effective-c++, extra/fast-by-value, 'Want Speed? Pass by Value.' archived article (read comments) much discussion (with many examples) for link to archived article (web.archive.org) -> consider all examples for item


//	(pre-c++11/item-20): best-practice to pass by const-ref, except: primatives / iterators / lambdas / std::*_function
//	<C++-11>: move-semantics, passing (an rvalue) by value, without copying it (only if callee takes ownership of the object)
//	[...]
//	Then comes the article (2009) (see below): Want Speed? Pass by Value.
//	<>
//	<(Pass by value if the callee was going to make a copy of the argument anyway, otherwise pass by const ref?)> <(Pass by-pointer if NULL must be an acceptable value)>

//	As to what will be the fastest, passing by value/reference/pointer: the only way to tell is to profile code

//	copy-elision: omit copy/move ctors, and their side effects, resulting in zero-copy pass-by-value semantics
//	return-value-optimization: <(something other than a case of copy-elision?)>
//	epsilon: <(not applicable to 'fast by-value', (a thing for) minimum floating point error?)>


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

//	<(Not a form of copy-elision?)> <(this is the case of modifying a variable mention in ongoings)>
//				Widget w;
//				foo(w);


//	Common limitations of copy-elision
//	LINK: https://docs.microsoft.com/en-us/previous-versions/ms364057(v=vs.80)?redirectedfrom=MSDN#nrvo_cpp05_topic3
//		multiple return points
//		conditional initializations

//	Most commercial-grade compilers support copy elision & (N)RVO (depending on optimization settings). C++17 makes many of the above classes of copy elision mandatory <which?>.


//	Standard reference:
//	{{{
//	When certain criteria are met, an implementation is allowed to omit the copy/move construction of a class object, even if the copy/move constructor and/or destructor for the object have side effects. In such cases, the implementation treats the source and target of the omitted copy/move operation as simply two different ways of referring to the same object, and the destruction of that object occurs at the later of the times when the two objects would have been destroyed without the optimization. (Because only one object is destroyed instead of two, and one copy/move constructor is not executed, there is still one object destroyed for each one constructed). This elision of copy/move operations, called copy elision, is permitted in the following circumstances (which may be combined to eliminate multiple copies):
//	in a return statement in a function with a class return type, when the expression is the name of a non-volatile automatic object (other than a function or catch-clause parameter) with the same cv-unqualified type as the function return type, the copy/move operation can be omitted by constructing the automatic object directly into the function's return value
//	in a throw-expression, when the operand is the name of a non-volatile automatic object (other than a function or catch-clause parameter) whose scope does not extend beyond the end of the innermost enclosing try-block (if there is one), the copy/move operation from the operand to the exception object (15.1) can be omitted by constructing the automatic object directly into the exception object
//	when a temporary class object that has not been bound to a reference (12.2) would be copied/moved to a class object with the same cv-unqualified type, the copy/move operation can be omitted by constructing the temporary object directly into the target of the omitted copy/move
//	when the exception-declaration of an exception handler (Clause 15) declares an object of the same type (except for cv-qualification) as the exception object (15.1), the copy/move operation can be omitted by treating the exception-declaration as an alias for the exception object if the meaning of the program will be unchanged except for the execution of constructors and destructors for the object declared by the exception-declaration.
//	}}}

//	automatic object: <(a variable on the stack)>

class A {};
void pass_A(A a) {}

//	Under certain conditions, the compiler is allowed to omit the copy/move-ctor, even if they have side effects. <(In such cases, the source/target are treated as two different ways of referring to the same object)>. (This means the number of objects that are constructed/destructed can differ depending on the implementation, however this is still always one object destroyed for each one constructed). Copy-elision is permitted in the following circumstances (which may be combined to eliminate multiple copies):
//	1)	In a return statement of a function with a class return type, when the expression is the name of a non-volatile automatic object (other than a function / catch-clause parameter), with the same cv-unqualified type as the function return type. <(Not a function with multiple return paths?)>. The copy/move is omitted by constructing the automatic object directly into the return value <(that is, the variable the function result is being assigned to)>.
A make_A() {
	A tmp;
	return tmp;
}
//	2)	In a throw-expression, when the operand (variable) is the name of a non-volatile automatic object (other than a function / catch-clause parameter) (whose scope does nto extend beyond the end of the innermost enclosing try-block). The copy/move is omitted by constructing the automatic object directly into the exception object.
//	<?>
//	3)	When a temporary class object that has not been bound to a reference would be copied/moved to a class object with the same cv-unqualified type. The copy/move is omitted by constructing the temporary object directly into the target variable.
void example_passA() {
	pass_A( make_A() );
}
//	4)	When the exception-declaration of an exception handler declares an object of the same type (except for cv-qualification) as the exception object. The copy/move is omitted by treating the exception-declaration as an alias for the exception object (if the meaning of the program will be unchanged by doing so, except for ctor/dtor calls).
void example_throwexpression() {
	try {
		throw logic_error("boom");
	//	Ongoing: 2022-05-14T17:32:31AEST example_throwexpression, 'e' must be 'logic_error' -> or would 'exception e' also get copy-elided?
	} catch (logic_error e) {	
		cout << "e=(" << e.what() << ")\n";
	}
}


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
	(void) t2;
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

//	Guideline: Don't copy your function arguments. Instead, pass them by value and let the compiler do the copying.
//	<(meaning, pass them by value ... if the callee was going to make a copy of them anyway?)>
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
//	RVO optimizations aren't required by any standard, but "recent versions of every compiler I've tested do perform these optimizations today."
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

//	LINK: https://stackoverflow.com/questions/270408/is-it-better-in-c-to-pass-by-value-or-pass-by-constant-reference
//	{{{
//	First answer (old-C++): pass by ref (const where possible), except for: primatives / iterators / lambdas / std::*_function
//	Second answer: Want Speed, Pass by Value. (see below) (link) for origional article.
//	Guidelines:
//		pass by ref:		If a function needs to modify the argument, pass as non-const ref
//		pass by value:		If the caller needs to make a copy of the argument, or passing a primative, pass by value
//		pass by pointer: 	If NULL is also an acceptable value (with/without const)
//		pass by const-ref:	Otherwise
//	Passing by-value makes explicit to the caller that a copy of the argument is being made
//	Passing by-value / by-const-ref make explicit that the origional value is not being modified
//	Passing by-ref implies the argument is not being copied


//	Example: by-ref / by-value / by-const-ref uses-cases
//	copy must be made of argument at some point - pass by-value
string to_upper(string s) {
	for (auto& c: s) { c = toupper(c); }
	return s;
}
//	do not need to copy/modify argument - pass by const-ref
bool all_upper(const string& s) {
	for (auto &c: s) { if (!isupper(c)) { return false; } }
	return true;
}
//	need to modify output stream to write to it, <(cannot pass stream by-value, streams are not copy-able)>
void write_helloworld(ostream& ss) {
	ss << "Hello World\n";
}

//	Pass by-value has certain optimization advantages, because the compiler can be sure the argument passed by-value is not a reference to another variable used elsewhere in the function.

//	pass by-value vs by-ref:
//	iterators
//		<(a question of optimization? no use for an iterator passed by const-ref?)>
//	lambdas / function-objects
//		<(a question of optimization? cheap to copy? ... are we not asking for nightmares vis-a-vis side-effects/state-variables, things one should not be ruling out?)> [...] (to-do for extra/lambdas-and-alternatives is best-practices for passing lambdas/function-objects)
//	streams <(do not copy streams?)>
//		<(it is meaningless to copy a stream, relevent link in worklog)>

//	Ongoings:
//	{{{
//	Ongoing: 2022-05-14T14:58:58AEST 'const string&' vs 'string const&' (?)
//	Ongoing: 2022-05-14T14:46:42AEST (advice is don't pass lambdas/std::function by-reference -> why should one pass a function-object by-value?) [...] a question of (more than) optimization?
//	}}}
//	}}}

//	LINK: http://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
//	{{{
//	Consider: Passing by-value can produce neater code.
//			vector<string> get_names() { ... }
//			const auto names = get_names();
//	vs:
//			void get_names(vector<string>& put_results) { ... }
//			vector<string> temp;
//			get_names(temp);
//			const auto names = move(temp);

//	<(rvalues allow move semantics)>

//	During return-value-optimization <(a form of copy-elision)>, the caller allocates space for the return value on its stack, and passes the address to the callee, which constructs the value to-be returned directly in the stack of the caller, eliminating the need for a copy <(or move, for <later-C++>)> for statements like:
//			auto names = get_names()

//	Normally when a parameter is passed by-value, a copy must be made <(copy-ctor must be called)>. 
//	However, when passing an rvalue, the caller uses the callee value, and the copy is elided(?) (and this is different/more-than just move-semantics?)
//			auto sorted_names1 = sorted( names );				//	must perform a copy of 'names'
//			auto sorted_names2 = sorted( get_names() );			//	'get_names()' can be copy(/move)-elided

//	<(copy-elision makes pass by-value practically in practice)>

//	Guideline: (once again), pass by-value instead of passing a ref and copying it


//	Example: copy-and-swap-assignment, 
//			T& operator=(const T& x) {
//				T tmp(x);
//				swap(*this, tmp);
//				return *this;
//			}
//	vs
//			T& operator=(T x) {
//				swap(*this, x);
//				return *this;
//			}
//	<(additional advantage is a single function to handle both lvalue/rvalue references?)> <(and copy/move-elision makes it efficent (sometimes)?)>
//	<(For copy-and-swap-assignment, Pass by-ref is unlikely to be slower, and it may be (considerably) faster than pass by-val (see extra/by-ref-vs-val-swap-assignment))> <(what is the takeaway from example?)>


//	Caravats:
//		<(prefer by-ref)>
//		<(compiler is unlikely to elide copying of function parameter if it is being used as a return value?)> 


//	Example: <(if one needs to return a by-value function parameter, get near-optimimum performance by swapping it into a default-constructed temp value and return that)>
vector<int> my_sorted(vector<int> names) {
	sort(names.begin(), names.end());
	vector<int> tmp;
	swap(tmp, names);
	//	or: 
	//auto tmp = move(names);
	return tmp;
}

//	(yet another example from comments)
//	{{{
//	Hi Dave, I tried to apply the idiom for copy assignment you describe, but I encountered one suspicious nuance when trying to specify exception specification for my copy assignment. I have a “gut feeling” that something is wrong, although I cannot clearly specify it. Here is my problem. Without passing by value I would specify my assigment like this:
//	
//	T& T::operator=( T const& x )
//	{ 
//	  T tmp(x);          // can throw
//	  swap(*this, tmp);  // no-throw (let's assume)
//	  return *this; 
//	}
//	This really says what I need to do to assign the value from one object represented by reference ‘x’. I need to make a copy first, and swap it with my value. Can this operation throw? Surely: a copy constructor is a typical place where one would expect a throw.
//	
//	Now, is the answer the same for the “pass-by-value” idiom?
//	
//	T& operator=(T x)    // this copying is not inside the assignment
//	{
//	    swap(*this, x);  // no-throw
//	    return *this;  
//	}
//	Technically we are doing the same thing, but copying is somehow ejected outside of the function. The function only does a no-fail (let’s assume that) swap. So in fact, I can write:
//	
//	T& operator=(T x) noexcept
//	{
//	    swap(*this, x);  // no-throw
//	    return *this;  
//	}
//	I am telling the truth: there is nothing in the copy assignment that could cause a throw. But anyone who tries to use the assignment may throw, beacuse our function even thouh it does not copy itself, forces you to copy T, even though you are not (or may be not) aware of it. That is, by declaring the function like this (with noexcept), while technically being correct, I confuse everyone by implying that using this assignment operator does not raise exceptions. I would be more honest if I wrote:
//	
//	T& operator=(T x) noexcept( std::is_nothrow_copy_constructible<T>::value )
//	{
//	    swap(*this, x);  // no-throw
//	    return *this;  
//	}
//	But this also looks strange: why would I base the condition on the properties of the constructor that I never call?
//	}}}

//	(yet another example from comments)
//	{{{
//		struct B { int i; };
//		struct D : B { int j; };
//		
//		void by_ref(B const & b)
//		{
//		}
//		void by_value(B b)
//		{
//		}
//		by_ref() takes “anything thay isa B” ie anything derived from B, whereas by_value() takes a B, and B only.
//		
//		… or so it seems. Actually, assuming a B copy-constructor of the form B::B(B const &), then by_value(d) still works, via implicit by_value(B::B(d)).
//		
//		B‘s copy constructor would need to be explicit to avoid this.
//		
//		Not sure how big of a deal that is, but passing by value (while using explicit constructors) could prevent slicing in some situations.
//		
//		(In fact, in general, the “slicing” of a Derived when constructing a Base from a Derived might be surprising in some situations. ie I suspect many people don’t think of their copy-constructor being used as a slicer. Or being ‘polymorphic’ in some way. Interesting…)
//	}}}

//	operator= is unconditionally noexcept
//	assignment from an lvalue is noexcept if T’s copy constructor is noexcept
//	assignment from an rvalue is noexcept if move-constructing a T is noexcept


//	Ongoing: 2022-05-14T16:53:12AEST <(I want to inform the innocent reader that integer, pointer and member pointer template parameters aren't lvalues either)>
//	Ongoing: 2022-05-14T16:20:06AEST 'my_sorted' -> using 'swap()' instead of move-constructing?
//	Ongoing: 2022-05-14T16:01:07AEST (a question asked elsewhere?) (relationship between copy-elision and move)
//	Ongoing: 2022-05-14T15:58:55AEST naming a variable to be used as an 'out' value -> 'put_results' vs 'out_results' vs <?>
//	}}}

//	LINK: https://stackoverflow.com/questions/35506708/move-constructor-vs-copy-elision-which-one-gets-called
//	{{{
//	elision of copy/move elision <(both are refered to as 'copy-elision')> is permitted in the following circumstances:
//	{{{
//	When a temporary class object that has not been bound to a reference would be copied/moved to a class object with the same type (ignoring cv-qualification), the copy/move operation can be omitted by constructing the temporary object directly into the target of the omitted copy/move.
//	In a return statement in a function with a class return type, when the expression is the name of anon-volatile object with automatic storage duration (other than a function parameter or a variable introduced by the exception-declaration of a handler) with the same type (ignoring cv-qualification) as the function return type, the copy/move operation can be omitted by constructing the automatic object directly into the function’s return value.
//	In a throw-expression, when the operand is the name of a non-volatile automatic object (other than a function or catch-clause parameter) whose scope does not extend beyond the end of the innermost enclosing try-block (if there is one), the copy/move operation from the operand to the exception object can be omitted by constructing the automatic object directly into the exception object.
//	When the exception-declaration of an exception handler declares an object of the same type (except for cv-qualification) as the exception object, the copy operation can be omitted by treating the exception-declaration as an alias for the exception object if the meaning of the program will be unchanged except for the execution of constructors and destructors for the object declared by the exception-declaration. There cannot be a move from the exception object because it is always an lvalue.
//	}}}
//	That is: <(provided by objects are of the same type?)>
//		1) 	assigning a temporary class object (rvalue) (not bound to a reference)
//		2)	returning an automatic/<temporary/rvalue?> value by-value
//		3)	throw -> catch() (requires exact same type?)
//		4)	<(When the exception-declaration of an exception handler declares an object of the same type (except for cv-qualification) as the exception object, the copy operation can be omitted by treating the exception-declaration as an alias for the exception object if the meaning of the program will be unchanged except for the execution of constructors and destructors for the object declared by the exception-declaration. There cannot be a move from the exception object because it is always an lvalue)>
//	Copy elision is forbidden in all other circumstances.
//	The number of return statements in a function has no bearing whatsoever on the legality of copy elision. However a compiler is permitted to not perform copy elision, even though it is legal, for any reason at all, including the number of return statements.
//	<(when a variable that is about to go out of scope is returned or thrown)>

//	Takeaway: copy-elision applies (only) to return, assignment, <(catching exceptions?)>, and <(exception handler declarations?)> <(not (ever) to making a copy of a named object?)>


//	<(<C++17> update:)>
//	<(there are now a few places where copy-elision is manditory)>
//	There are now a few places where copy elision is mandatory. If a prvalue can be bound directly to a by-value function parameter, or a by-value return type, or to a named local variable, copy elision is mandatory in C++17. This means that the compiler shall not bother even checking for a copy or move constructor

//	Example: Legal <C++17>, copy-elision with deleted copy-ctor/assign
struct X {
    X() = default;
    X(const X&) = delete;
    X& operator=(const X&) = delete;
};
X foo(X) {
    return X{};
}
void example_elisionDeletedCopy() {
    X x1 = foo( X{} );
	(void) x1;
}


//	Ongoing: 2022-05-14T17:38:38AEST test if copy-elision is occuring - can a class with a deleted copy-ctor/assign be copied?
//	}}}

//	LINK: https://stackoverflow.com/questions/9444485/why-is-rvo-disallowed-when-returning-a-parameter
//	{{{
//	RVO copy-elision is disabled when returning a function parameter (ora catch-clause parameter)

//	Explanation:
//	{{{
//	Imagine no_rvo is defined in a different file than main so that when compiling main the compiler will only see the declaration
//	
//	X no_rvo(X x);
//	and will have no idea whether the object of type X returned has any relation to the argument. From what it knows at that point, the implementation of no_rvo could as well be
//	
//	X no_rvo(X x) { X other; return other; }
//	So when it e.g. compiles the line
//	
//	X const& x = no_rvo(X());
//	it will do the following, when maximally optimizing.
//	
//	Generate the temporary X to be passed to no_rvo as argument
//	call no_rvo, and bind its return value to x
//	destruct the temporary object it passed to no_rvo.
//	Now if the return value from no_rvo would be the same object as the object passed to it, then destruction of the temporary object would mean destruction of the returned object. But that would be wrong because the returned object is bound to a reference, therefore extending its lifetime beyond that statement. However simply not destructing the argument is also no solution because that would be wrong if the definition of no_rvo is the alternative implementation I've shown above. So if the function is allowed to reuse an argument as return value, there can arise situations where the compiler could not determine the correct behaviour.
//	
//	Note that with common implementations, the compiler would not be able to optimize that away anyways, therefore it is not such a big loss that it is not formally allowed. Also note that the compiler is allowed to optimize the copy away anyway if it can prove that this doesn't lead to a change in observable behaviour (the so-called as-if rule).
//	}}}
//	}}}

//	LINK: https://shaharmike.com/cpp/rvo/
//	{{{
struct Snitch {   // Note: All methods have side effects
  Snitch() { cout << "c'tor" << endl; }
  ~Snitch() { cout << "d'tor" << endl; }
  Snitch(const Snitch&) { cout << "copy c'tor" << endl; }
  Snitch(Snitch&&) { cout << "move c'tor" << endl; }
  Snitch& operator=(const Snitch&) {
    cout << "copy assignment" << endl;
    return *this;
  }
  Snitch& operator=(Snitch&&) {
    cout << "move assignment" << endl;
    return *this;
  }
};

//	RVO makes returning objects free <(and assigning objects free)>
//	It works by allocating memory for the to be returned <(or assigned)> in the caller's stack-frame, and passing the callee a pointer so it can construct the object in question there.

//	copy-elision restricitons <((by which they mean) RVO restrictions?)>

//	Example: copy-elision is unlikely to work with multiple return paths (although C++ does not require it be disabled in this case)
Snitch CreateSnitch(bool runtime_condition) {
  Snitch a, b;
  if (runtime_condition) {
    return a;
  } else {
    return b;
  }
}
Snitch make_Snitch() {
	return Snitch();
}
void example_createSnitch() {
	cout << "CreateSnitch(true)\n";
	Snitch s1 = CreateSnitch(true);
	cout << "make_Snitch()\n";
	Snitch s2 = make_Snitch();
}

//	There is no way to apply RVO when returning a value not local to the scope of the returning function.


//	Guideline: returning by 'move()' is wrong most of the time <(this is an item?)>
//	Returning by 'move()' disables RVO, forcing the move-ctor. The result is likely less optimial than simply returning by-value without move.


//	RVO can only happen when an object is created from a returned value. copy-elision is not applicable when assigning returned values to an existing object.


//	Example: <(in some cases, an unnamed variable can't be RVO-ed)> <(unnamed-member-variable/all-member-variables cannot be RVO-ed?)> 
struct Wrapper { Snitch s; };
Snitch make_SnitchFromWrapper() {
	return Wrapper().s;
}
void example_memberVariableRVO() {
	Snitch s = make_SnitchFromWrapper();
}


//	Ongoing: 2022-05-14T18:08:55AEST clarify example, ((just?) unnamed) member variables cannot be RVO-ed
//	Ongoing: 2022-05-14T18:00:56AEST 'Snitch' -> a shorter report than multiple lines for ctors/dtors (a report upon destruction, how many ctor/dtors were called to get us here?)
//	}}}

//	C++ Reference: copy-elision
//	LINK: https://en.cppreference.com/w/cpp/language/copy_elision
//	{{{
//	<(where the rules are as long-winded as they are complete)>
//	}}}

//	LINK: https://docs.microsoft.com/en-us/previous-versions/ms364057(v=vs.80)?redirectedfrom=MSDN#nrvo_cpp05_topic3
//	{{{
//	<(limitations examples)>
//	}}}



//	Example: copy-elision and a deleted copy-ctor/assign
//	<(is allowed?)> <(and could even be our test, for whether copy-elision is definitely taking place)>

//	Example: modifying a value that has been copy-elided
//	<(copy-elision is not applicable where a copy must be made?)>

//	Example: verify copy-elision is happening(?)
//	<(using deleted copy-ctor/assign (see above))>

//	Examples: when copy-elision will not be applied
//	<(multiple return paths)>
//	<(return variable not in callee scope)>
//	<(wrong <narrowed?> type?)>
//	<?>

int main()
{
	example_multiple_valid_outputs();
	cout << "\n";

	example_make_Thing();
	//cout << "\n";

	write_helloworld(cout);
	cout << "\n";

	example_createSnitch();
	cout << "\n";

	example_memberVariableRVO();
	cout << "\n";

	return 0;
}

