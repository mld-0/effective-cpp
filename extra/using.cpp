#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;
//	Ongoing: 2022-04-25T23:50:33AEST the multiple roles of 'using'?

//	LINK: https://stackoverflow.com/questions/20790932/what-is-the-logic-behind-the-using-keyword-in-c
//	The role of 'using':
//		template aliases (or template typedefs (the former is preferred namewise))
//		namespace aliases (i.e: 'namespace PO = boost::program_options')
//		a typedef declaration can be viewed as a special case of non-template alias-declaration
//		bringing something into scope (i.e: std into global scope) <(member functions, inheriting constructors)>

//	it cannot be used for: <>
int i;			
//using r = i;						//	error
//	instead:
using r = decltype(i);				//	<C++?>
//	it also cannot be used to name a set of overloads:
using std::cos;						//	bring 'cos' into scope
//using std::cos(double);			//	error
//using test = std::cos(double);	//	error (use function pointer alias instead (see below))


//	<C++11> the 'using' keyword when used for 'type alias' is identical to 'typeref'
//	A typedef-name can also be introduced by an alias-declaration. The identifier following the using keyword becomes a typedef-name and the optional attribute-specifier-seq following the identifier appertains to that typedef-name. It has the same semantics as if it were introduced by the typedef specifier. In particular, it does not define a new type and it shall not appear in the type-id.
//	Examples:
typedef void (*PF1)(double);		//	C style typedef, make 'PF1' a pointer to a func returning void and accepting double
using PF2 = void (*)(double);		//	'using' base equivalent
//using PF3 = [](double)->void;		//	error: using plus suffix return type
using PF4 = auto(double)->void;		//	<(fixed thanks to DyP)>


//	<Pre-C++11> the 'using' keyword can bring member functions into scope. 
//	<C++11> can also do this for constructors
class Base {
public:
	void f(double);
};
class Derived: public Base {
public:
	using Base::f;		//	lift Base f into Derived's scope
	void f(char);
	void f(int);

	using Base::Base;	//	lift Base constructor into Derived's scope <C++11> <(this provides a default constructor for Derived? If Base has a default constructor?)>
	Derived(char);
	Derived(int);
};


template<typename T>
class MyAlloc {};
template<typename T, class A>
class MyVector {};

//	Consider:
//	<pre-C++11> declaring a typedef in a template struct was the <workaround> for declaring a template typedef(?)
template<typename T>		
struct Vec_i {
	typedef MyVector<T,MyAlloc<T>> type;
};
Vec_i<int>::type p1;
template<typename T> void foo_i(typename Vec_i<T>::type& x) {} 	//	Ongoing: 2022-02-14T16:59:09AEDT second 'typename' not in stackoverflow example
//	using allows alias declarations (a generalization of typedef), which supports template parameters
template<typename T>
using Vec_ii = MyVector<T,MyAlloc<T>>;
Vec_ii<int> p2;
template<typename T> void foo_ii(Vec_ii<T>& x) {}
//	Ongoing: 2022-02-14T17:06:08AEDT 'Vec<i/ii?>' is not an alias for a type, and should not be tkane for a typedef-name

//	Ongoing: 2022-02-14T17:04:08AEDT (should we be prefering 'using' <alias declarations> to typedefs?)

//	An alias-declaration is a declaration and not a definition. An alias declaration introduces a name into a declarative region as an alias for the type designated by the rhs of the declaration. 


int main()
{
	unique_ptr<Derived> d1(new Derived); 
	d1 = nullptr;


	double a1 = 7;
	double a2 = cos(a1);
	a2 = 4;

	//	<(A difference between typedef and using)>
	using vector3d_t = double[3];
	vector3d_t v1 = {1,2,3};
	v1[1] = 4;
	//typedef double[3] vector3d_t;		//	error (typedef cannot declare an array)

	return 0;
}

