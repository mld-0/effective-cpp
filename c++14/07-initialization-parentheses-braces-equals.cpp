//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

class Item {
public:
	Item() {};
	Item(int x) : data(x) {};
	int data;
	void speak() { cout << "(x,y)=(" << x << "," << y << ")\n"; }
private:
	int x{0};
	int y = 0;
	//int z(0);			//	error, cannot use parenthesis to initalize class variables
};

class Widget {
public:
	Widget() 					{ cout << "\tdefault-ctor\n"; }
	Widget(const Widget& b) 	{ cout << "\tcopy-ctor\n"; }
	Widget(Widget&& b) 			{ cout << "\tmove-ctor\n"; }

	Widget(int i, bool b) 		{ cout << "\t(int,bool)=(" << i << "," << b << ")\n"; }
	Widget(int i1, int i2) 		{ cout << "\t(int,int)=(" << i1 << "," << i2 << ")\n"; }
	Widget(int i, double d) 	{ cout << "\t(int,double)=(" << i << "," << d << ")\n"; }

	Widget(std::initializer_list<int> il) 
	{ cout << "\til=("; for (const auto& x: il) { cout << x << ","; } cout << ")\n"; }

	Widget(const Widget& w, int x) 	{ cout << "\t(Widget,int)=(" << *this << "," << x << ")\n"; }

	Widget& operator=(const Widget& w) 	{ cout << "\tcopy-assignment\n"; return *this; }
	Widget& operator=(Widget&& w) 		{ cout << "\tmove-assignment\n"; return *this; }

	operator int() const 	{ return 7; }
};

//	TODO: 2022-02-24T20:01:29AEDT effective-c++, item 07, initalialization-parenthesis-braces-equals, parenthesis-vs-braces as a template author ((specifically <for/regarding>) the variadic template example?) 
template<typename T, typename... Ts>
void doSomeWork(Ts&&... params) {
	//	<Implementation using parenthesis>
	//T localObject(std::forward<Ts>(params)...);
	//	<Implementation using braces>
	//T localObject{std::forward<Ts>(params)...);
}
//	Calling code:
//vector<int> v;
//doSomeWork<vector<int>>(10,20)

//	TODO: 2022-02-24T20:04:41AEDT effective-c++, extra/variadic-functions
//	TODO: 2022-02-24T20:05:04AEDT effective-c++, extra/variadic-template

int main()
{
	//	If the object conceptually holds the value(s) being passed, use braced initialization {}. If supplying constructor function arguments, use parenthesis (). For the default constructor use empty braces {}.
	//	Braced initialization is called 'uniform', since compared with parenthesis/equals it can be used in the most contexts. <(Some say to prefer it everywhere)>.

	//	Example: initializing integers using equals, parenthesis, braces, and equals-and-braces.
	int iZero1 = 0;
	int iZero2(0);
	int iZero3{0};
	int iZero4 = {0};
	//int iZero5 = {0,0};			//	error, excess elements in scalar initializer
	cout << "(iZero1,iZero2,iZero3,iZero4)=(" << iZero1 << "," << iZero2 << "," << iZero3 << "," << iZero4 << ")\n";

	//	Example: initializing auto variables as integers <()>
	//	<(The only situation where = is preferred over {} is when using auto keyword to get the type determined by the initializer)>.
	auto aZero1 = 0;
	auto aZero2(0);
	auto aZero3{0};
	auto aZero4 = {0};			//	incorrect, deduced type is initalizer_list<int>
	cout << "(aZero1,aZero2,aZero3)=(" << aZero1 << "," << aZero2 << "," << aZero3 << ")\n";
	cout << "aZero4.size()=(" << aZero4.size() << ")\n";
	cout << "\n";


	//	Braced {} initalization prohibits implicit narrowing conversions: (it is an error if the value of an expression in a braced initalizer isn't guaranteed to be expressible by the type of object being initalized). (Parenthesis and equals do not check for such implicit narrowing. <(when do they warn for it?)> 
	double dval1 = 1.3;
	double dval2 = 5.4;
	double dval3 = 2.0;
	int dvals_sum1 = dval1 + dval2 + dval3;
	int dvals_sum2( dval1 + dval2 + dval3 );
	//int dvals_sum3{ dval1 + dval2 + dval3 };		//	error, double cannot be narrowed to int in initializer_list
	//int dvals_sum4 = { dval1 + dval2 + dval3 };	//	error, double cannot be narrowed to int in initializer_list


	//	Example: C++'s 'most vexing parse', initialization with empty parenthesis () interpreted as function declaration
	Item iEmpty1;			//	no parenthesis, <(implicit)> default construction
	//Item iEmpty2();		//	incorrect, declares a function
	Item iEmpty3{};			//	braces, <(explicit)> default construction


	//	Example: distinguish between copy/move construction/assignment for each kind of initalization
	//	Note: braced {} initialization strongly prefers initializer_list (if an initializer_list ctor is available, even if a ctor with better matching arguments is exists, we consider that better-matching-ctor only if conversion of values in braces {} to initializer_list is impossible). (Braces {} and parenthesis () have the same meaning for constructor calls as long as initializer_list parameters are not involved).
	//	Adding a initializer_list ctor to an existing class can change the behaviour of existing braced {} initializations for that class.
	cout << "default-ctor:\n";
	Widget w1;					
	Widget w2{};			
	Widget w3 = {};					
	cout << "copy-ctor:\n";
	Widget wCopy1 = w1;
	Widget wCopy2(w1);
	Widget wCopy3{w1};
	Widget wCopy4 = {w1};
	cout << "initializer_list:\n";
	Widget wInit1 = {w1, 2};			//	{7,2}
	Widget wInit2( {w1, 2} );			//	{7,2}
	Widget wInit3{ {w1, 2} };			//	{7,2}
	Widget wInit4 = {3,true};			//	{3,1}
	Widget wInit5{w1,2};				//	{7,2} 	note: initializer_list is chosen over a ctor w/ better matching types
	Widget wInit6{w1,true};				//	{7,1}
	cout << "empty initializer_list:\n";
	Widget wEInit1( {} );				//	{}		note: different behaviour for () vs {}
	Widget wEInit2{ {} };				//	{0}
	Widget wEInit3 = { {} };			//	{0}
	//	Ongoing: 2022-02-24T05:37:58AEDT (another book mistake) 'Widget wEInit2{{}}' (book suggests 'Widget w6{{}};' calls initializer_list ctor with empty list -> list is actually {0} (explaining this behaviour is to-do)
	//	Ongoing: 2022-02-24T05:11:30AEDT (book suggests) 'Widget WEInit4{w3}' should be hijacked by the initializer_list<int> ctor (since there is a conversion 'operator() int' defined for Widget) -> (but this doesn't appear to be the case) do they mean when there is no explicit copy-ctor defined) (that is, does the example fail copied from the book?)
	//Widget wEInit4{w3};
	cout << "Widget(w,int):\n";
	Widget wWI1(w1,2);
	cout << "Widget(int,bool):\n";
	Widget wIB1(3,true);
	cout << "Widget(int,int):\n";
	Widget wII1(3,5);
	cout << "Widget(int,double):\n";
	Widget wID1(3,3.5);
	//Widget wID2{3,3.5};				//	error, cannot narrow value in initializer_list <(should call Widget(int,double)?)>
	cout << "\n";
	//	Ongoing: 2022-02-24T03:58:47AEDT 'Widget wID2{3,3.5}' (say what) the rules about looking up better-matching-ctors when conversions to initializer_list is impossible (narrowing double->int is 'valid' behaviour in general, but not in a initializer_list, hence we can't convert to initializer_list, but we also can't look for better-matching-ctor because said conversion is possible under other circumstances)
	//	Ongoing: 2022-02-24T04:01:13AEDT this ambigious behaviour would seem to argue against 'universal' use of braced initializers -> (it would seem to argue against its use entirely) -> (do the rules make more sense when laid out), <(From stackoverflow, (see below))>
	//	{{{
		//	LINK: https://stackoverflow.com/questions/18222926/why-is-list-initialization-using-curly-braces-better-than-the-alternatives
		//	1) "Prefer {} initialization over alternatives unless you have a strong reason not to", 
		//	2) If the object I'm creating conceptually holds the values I'm passing in the constructor (e.g. containers, POD structs, atomics, smart pointers etc.), then I'm using the braces. If the constructor resembles a normal function call (it performs some more or less complex operations that are parametrized by the arguments) then I'm using the normal function call syntax. For default initialization I always use curly braces. For one, that way I'm always sure that the object gets initialized irrespective of whether it e.g. is a "real" class with a default constructor that would get called anyway or a builtin / POD type. Second it is - in most cases - consistent with the first rule, as a default initialized object often represents an "empty" object. In my experience, this ruleset can be applied much more consistently than using curly braces by default, but having to explicitly remember all the exceptions when they can't be used or have a different meaning than the "normal" function-call syntax with parenthesis (calls a different overload). It e.g. fits nicely with standard library-types like std::vector
		//	3) "There are MANY reasons to use brace initialization, but you should be aware that the initializer_list<> constructor is preferred to the other constructors, the exception being the default-constructor. This leads to problems with constructors and templates where the type T constructor can be either an initializer list or a plain old ctor, Assuming you don't encounter such classes there is little reason not to use the intializer list")>
	//	}}}
	//	<(Only if there is no way to convert the types of arguments in a braced initializer to the type in a std::initializer_list do compilers fall back on normal overload resolution)>

	//	Takeaway from Above:
	//Widget wT1{w1,2};					//	calls initializer_list ctor
	//Widget wT2(w1,2);					//	calls (Widget,int) ctor

	//	TODO: 2022-02-24T04:17:38AEDT effective-c++, item 07, initialization-parenthesis-braces-equals, (if they aren't impossible to read as-such) the parenthesis/braces (and equals?) rules for initialization
	//	<(initializer_list is used in every 'vector<int>' example (see below) that uses braces at all (not enough of an observation to explain) difference between '{{}}' and '({})'? do rules-as-presented (see above) explain this?)>


	//	Example: std::vector, equals vs parenthesis () vs braces {}
	vector<int> vin1 = {10, 20};			//	initializer_list = {10,20}
	vector<int> vin2(10, 20);				//	vector<int>(size,val) = {20,20,20,20,20,20,20,20,20,20}
	vector<int> vin3{10, 20};				//	initializer_list = {10,20}
	vector<int> vin4 = { {10, 20} };		//	initializer_list = {10,20}
	vector<int> vin5( {10, 20} );			//	initializer_list = {10,20}
	vector<int> vin6{ {10, 20} };			//	initializer_list = {10,20}
	cout << "vin1=("; for (const auto& x: vin1) { cout << x << ","; } cout << ")\n";
	cout << "vin2=("; for (const auto& x: vin2) { cout << x << ","; } cout << ")\n";
	cout << "vin3=("; for (const auto& x: vin3) { cout << x << ","; } cout << ")\n";
	cout << "vin4=("; for (const auto& x: vin4) { cout << x << ","; } cout << ")\n";
	cout << "vin5=("; for (const auto& x: vin5) { cout << x << ","; } cout << ")\n";
	cout << "vin6=("; for (const auto& x: vin6) { cout << x << ","; } cout << ")\n";
	cout << "\n";

	//	Example: <(the problem of '{}' vs '{0}')> for vector initalialization
	//	Ongoing: 2022-02-24T05:29:06AEDT (equivalency of) '({})' with '= {}' (explaining {} vs {0} for '{{}}' vs '({})' <(Explain the (relationships-describing) following pattern)>
	//	Ongoing: 2022-02-25T00:40:43AEDT note the equivalency: '( int{} )' = '( {} )' and '{ int{} }' = '{ {} }'
	vector<int> vEmpty1;					//	{}
	vector<int> vEmpty2 = {};				//	{}
	vector<int> vEmpty3{};					//	{}
	vector<int> vEmpty4( {} );				//	{}
	vector<int> vEmpty5( int{} );			//	{}
	vector<int> vEmpty6 = { {} };			//	{0}
	vector<int> vEmpty7{ {} };				//	{0}
	vector<int> vEmpty8{ int{} };			//	{0}
	vector<int> vEmpty9( { {} } );			//	{0}	
	vector<int> vEmpty10( { int{} } );		//	{0}	
	cout << "vEmpty1=("; for (const auto& x: vEmpty1) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty2=("; for (const auto& x: vEmpty2) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty3=("; for (const auto& x: vEmpty3) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty4=("; for (const auto& x: vEmpty4) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty5=("; for (const auto& x: vEmpty5) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty6=("; for (const auto& x: vEmpty6) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty7=("; for (const auto& x: vEmpty7) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty8=("; for (const auto& x: vEmpty8) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty9=("; for (const auto& x: vEmpty9) { cout << x << ","; } cout << ")\n";
	cout << "vEmpty10=("; for (const auto& x: vEmpty10) { cout << x << ","; } cout << ")\n";
	cout << "\n";


	//	Ongoing: 2022-02-24T03:45:10AEDT error message is "cannot find deleted destructor" for 'atomic<int> iAtomic3' (how to declare a class that can be initalized with braces/parenthesis but not equals?) <mechanism?>
	//	Uncopyable objects (e.g: std::atomics (see item 40)) may be initialized using braces {} or parenthesis () (but not equals)
	std::atomic<int> iAtomic1{0};
	std::atomic<int> iAtomic2(0);
	//std::atomic<int> iAtomic3 = 0;		//	error (cannot initalize with equals), <(error message refers to a deleted ctor?)>

	//	<(Book suggestion: "design your constructors so that the overload called isn't affected by whether clients use parentheses or braces" (in other words) don't use initalizer list ctors?)>
	//	<(Most devs chose one kind <of initalialization> and use others when they have to)>

	//	Ongoing: 2022-02-24T04:15:51AEDT previous material (see below), (is there) anything not covered (better) above?
	//	{{{
	////	Example: Braced initalialization strongly prefers overloads taking std::initializer_list (Note how it is selected even though there are other constructors which are a better match for the types of both arguments)
	//Widget b1(10, true);			//	Widget(int,bool)
	//Widget b2(10, 5);			//	Widget(int,int)
	//Widget b3(10, 5.0);			//	Widget(int,double)
	////cout << "\n";
	//Widget b4{10, true};			//	Widget(initializer_list<int>)
	//Widget b5{10, 5};			//	Widget(initializer_list<int>)
	////Widget b6{10, 5.0};		//	error, requires narrowing conversions
	////cout << "\n";
	//Widget b7(b1);				//	Widget(const Widget&)	
	//Widget b8(move(b1));			//	Widget(Widget&&)
	////cout << "\n";
	//Widget b9{b1};				//	Widget(const Widget&)		<(book claims Widget(initializer_list<int>) should be called?)>
	//Widget b10{move(b1)};		//	Widget(Widget&&)			<(book claims Widget(initializer_list<int>) should be called?)>
	//cout << "\n";
	//Widget b11(b1, b2);			//	Widget(int,int)
	//Widget b12{b1, b2};			//	Widget(initializer_list<int>)
	//cout << "\n";
	//Widget b13;					//	Widget()
	//Widget b14{};				//	Widget()
	////Widget b15();				//	most-vexing-parse, declares the function 'b15' which returns 'Widget'
	////cout << "\n";
	////	Ongoing: 2022-02-23T20:27:33AEDT why b16 receives an empty initializer_lists, while b17 receives an initializer list containing 0 (book claims both should be empty)?
	//Widget b16( {} );				//	Widget(initializer_list<int>)
	//Widget b17{ {} };				//	Widget(initializer_list<int>)
	////cout << "\n";
	//vector<int> vec5 = {};
	//vector<int> vec3( {} );		//	vec3={}
	//vector<int> vec4{ {} };		//	vec4={0}
	//cout << "vec3=("; copy(vec3.begin(), vec3.end(), std::ostream_iterator<int>(std::cout, " ")); cout << ")\n";
	//cout << "vec4=("; copy(vec4.begin(), vec4.end(), std::ostream_iterator<int>(std::cout, " ")); cout << ")\n";
	//cout << "vec5=("; copy(vec5.begin(), vec5.end(), std::ostream_iterator<int>(std::cout, " ")); cout << ")\n";
	//cout << "\n";
	//	}}}
	//	unused variables
	(void) iAtomic1; (void) iAtomic2; (void) dvals_sum1; (void) dvals_sum2;
	return 0;
}

//	Summary:
//		If the object conceptually holds the value(s) being passed, use braced initialization {}. If supplying constructor function arguments, use parenthesis (). For the default constructor, empty braces {}.
//		During ctor overload resolution, braced initalizers strongly prefer initializer_list if at all possible, even if other ctors have better matching argument types.
//		The difference between initalization with braces/parenthesis is highlighted by 'vector<int> v1(10,20)={20,20,20,20,20,20,20,20,20,20}' and 'vector<int> v2{10,20}={10,20}'.
//		<template usage/design>

