//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	special member functions: functions C++ is willing to generate on its own. 
//	Implicitly public and inline.
//	<C++98>: default-ctor, default-dtor, copy-ctor, copy-assign
//	<C++11>: (adds) move-ctor, move-assign

//	{{{
//	Ongoing: 2022-04-27T21:45:48AEST and by 'default ctor/dtor do nothing', we mean, they implicitly do everything an empty explicit function would do?
//	Ongoing: 2022-04-27T21:48:33AEST intuition vis-a-vis special member function rules of generation?
//	Ongoing: 2022-04-27T21:48:58AEST are there non-special member functions which can be used with '= default' / '= delete'?
//	Ongoing: 2022-04-27T21:52:04AEST class with default all copy/move ctor/assign, and an uncopyable, unmoveable static member variable?
//	Ongoing: 2022-04-27T21:52:56AEST <something-something> copy-move ctor/assign being implemented in terms of the other?
//	Ongoing: 2022-04-27T22:12:35AEST <specifying/example-how-to> perform a shallow vs deep copy in C++ (can one do either? which one performs is a matter of the implementation of copy/move-ctor/assign?) (default copy-move/assign performs a shallow copy) [...] (is it meaningful to ask about shallow vs deep move?)
//	Ongoing: 2022-04-27T22:22:56AEST how hard 'ExampleDeep' would be with 'char* ptr' instead of 'string* ptr'? -> (you tell me, how f----- hard is it to copy a block of memory (being a member variable, we have access to its size?) (can one write an entire item on 'copy what is at char*'?))
//	}}}

//	LINK: https://stackoverflow.com/questions/184710/what-is-the-difference-between-a-deep-copy-and-a-shallow-copy
//	shallow copy: memberwise copy of values and pointers
//	deep copy: memberwise copy of values, pointers are duplicated and deep-copied
//	<(shallow copying an object containing pointers is probably a bad idea?)> <(a class with pointer member variables should probably implement a custom, deep copy copy-ctor?)>

//	Example: implement deep copy
struct ExampleDeep {
	string* ptr;
	~ExampleDeep() { delete ptr; }
	ExampleDeep(const string& str) : ptr(new string(str)) {}
	ExampleDeep(const ExampleDeep& old) : ptr(new string(*old.ptr)) {}
};


//	If depending on implicit copy/move-ctor/assign, one should explicitly define them '= default'. 
//	Among other reasons, because later additions to a class can (such as a custom dtor) disable previously-enabled implicit special member functions, (either breaking things, or silently causing all moves to be replaced with copies (with potentially disasterous performance implications)).


//	Rules of Generation: special member functions are not generated unless they are needed
//	The default ctor/dtor ~~do nothing~~ are equivalent to empty functions
//	default copy-ctor/assign performs a memberwise copy 
//	default move-ctor/assign performs a memberwise move
//	memberwise copy/move is applied to all non-static variables (which must support copy/move)
//	These default special member functions can be explicitly enabled/disabled with '= default' / '= delete'
struct Widget {
//	default-ctor: if no explicit ctor
	Widget();

//	default-dtor: if no explicit dtor, virtual if necessary
	~Widget();

//	copy-ctor/move-ctor: if no explicit move-ctor/assign
	Widget(const Widget&);
	Widget& operator=(const Widget&);

//	move-ctor/move-assign: if no explicit copy/move/dtor
	Widget(Widget&&);
	Widget& operator=(Widget&&);
};
//	It is deprecated but possible for the compiler to produce an either of an implicit copy-ctor/assign from the other. This is not true for move-ctor/assign.






//	TODO: 2022-04-27T23:25:59AEST effective-c++, item 17, implicit-special-member-functions, member variable 'int* x = 0' -> has a value before ctor is reached? (behaviour when assigning value to both variable and ctor initalization list) (better practice for initalizing the value of a class variable (is there not some f----- of values being re-used?) (is there an item on class variable initalization?)
//	Ongoing: 2022-04-28T00:17:27AEST (another perfectly valid alternative would have been to swap the pointers (for move-assign)) (not a solution for copy-assign (since that leaves noone to delete 'x') (unless, we pass by-value instead of by reference, and swap '*this' with the argument?) (where it would be swapped with a temp var passed-by-value and deleted by that))
//	{{{
//	Ongoing: 2022-04-27T21:36:30AEST move-ctor *that is, only if no explicit copy-ctor, (a class can have both implicit move-ctor and copy-ctor?)
//	Ongoing: 2022-04-27T21:42:08AEST (verify, the above ('Widget') (special member function) signatures are correct?)
//	Ongoing: 2022-04-27T22:33:37AEST ('cplusplus.com' lies?) -> an explict copy-move/ctor does not prevent an implicit move-ctor/assign [...] -> (and it lies again(?)) (each of move-ctor/assign can be generated from the other one?) [...] (or are these lies our compiler being generous, ought we not assume any implicit function not allowed by the rules above), (another source for these rules?) [...] or is it because 'Fruit c = move(b)' invokes move-ctor not move-assign(?) (same as 'Fruit f2 = c' invokes copy-ctor not copy-assign?)
//	Ongoing: 2022-04-27T23:05:09AEST findings: each of copy-ctor/move can only be defined in terms of the other if there is is no explicit move, implicit move is possible regardless of the implementation of copy, but an explicit move-ctor/assign prohibits the other (and if the move-ctor/assign is unavailable, the alternative copy-ctor/assign will be used) [...] actually, no move is generated where an explicit copy is defined -> moves are being turned into copies (move can become copies, copies cannot become moves)
//	Ongoing: 2022-04-27T23:15:29AEST how would self-assignment cause problems for what we have implemented below?
//	Ongoing: 2022-04-27T23:20:55AEST our assignments use 'delete', our ctors do not -> (can we expect) problems implementing ctors in terms of assignemnts (what wikipedia example does, a best practice) [...] (we would be deleting 'this.x' before ever assigning anything to it? ... if we take the time to set it '= 0' first, we can safely delete it?) (what if we initalize it to 0?)
//	If a custom copy is needed (it stands to reason) a custom move is needed
//	}}}
class Fruit {
	int* x = 0;
public:
	~Fruit() { delete x; }
	Fruit(int val) : x(new int(val)) { cout << "Fruit(int)\n"; }
	Fruit(const Fruit& old) { cout << "Fruit(Fruit&)\n"; *this = old; }
	Fruit& operator=(const Fruit& old) { cout << "operator=(Fruit&)\n"; delete x; x = new int(*old.x); return *this; }
	Fruit(Fruit&& old) { cout << "Fruit(Fruit&&)\n"; *this = move(old); }
	Fruit& operator=(Fruit&& old) { cout << "operator=(Fruit&&)\n"; delete x; x = old.x; old.x = 0; return *this; }
};
//	note: '*this = x' invokes copy-assign, '*this = move(x)' invokes move-assign


//	Example: <(The existance of template versions of template versions of special member functions does not prevent the creation of implicit ones (of the same signiture (or was the default ctor a special case))?)> (item 26 demonstrates this can have important consiquences?) [...] (that is not true: a template copy-ctor deletes the default ctor (but it does not delete the default copy-ctor))
struct A {
	//	does not generate default-ctor
	A() = default;
	template<typename T>
	A(const T& rhs) { cout << "A(T&)\n"; }
	//	does generate default copy-ctor:
	//A(const A&) = default;
};


//	Example: Wikipedia, Explicit vs Implicit
//	{{{
//	LINK: https://en.wikipedia.org/wiki/Special_member_functions
class Explicit {
 public:
  Explicit() { std::cout << "Default constructor " << message_ << '\n'; }

  explicit Explicit(std::string message) : message_(std::move(message)) {
    std::cout << "Non-default constructor " << message_ << '\n';
  }

  Explicit(const Explicit& other) {
    std::cout << "Copy constructor " << message_ << '\n';
    *this = other;  // invoke copy assignment operator
  }

  Explicit& operator=(const Explicit& other) {
    std::cout << "Copy assignment operator " << message_ << '\n';
    if (this != &other) {
      message_ = other.message_;
    }
    return *this;
  }

  Explicit(Explicit&& other) noexcept {
    std::cout << "Move constructor " << message_ << '\n';
    *this = std::move(other);  // invoke move assignment operator
  }

  Explicit& operator=(Explicit&& other) noexcept {
    std::cout << "Move assignment operator " << message_ << '\n';
    if (this != &other) {
      message_ = std::move(other.message_);
    }
    return *this;
  }

  ~Explicit() { std::cout << "Destructor " << message_ << '\n'; }

 private:
  friend class Implicit;
  std::string message_;
};

class Implicit : public Explicit {
 public:
  void Spew() {
    std::cout << "Implicit(" << message_ << ", " << member_.message_ << ")\n";
  }

 private:
  Explicit member_;
};
//	}}}

//	Rule of Zero:
//	Classes that have any explicit special member functions should not deal exclusively with ownership.
//	(That is, classes that have any explicit special member functions should be (and only be) resource managers).
//	(From the single responsibility priniple).
//	(other classes (classes that are not resource managers) should not have explicit special member functions).

//	Rule of Three:
//	If one is declaring copy-ctor/copy-assign/dtor, one should declare all 3. A non-default dtor implies non-default copy operations.

//	Rule of Five:
//	LINK: https://stackoverflow.com/questions/45754226/what-is-the-rule-of-four-and-a-half
//	Add move-ctor/assign. (move support is generally an optimization, not a requirement).
//	<(some sources state 'swap()' instead of move-assign)>

//	Ongoing: 2022-04-28T01:04:34AEST (methods described by) talk of achieving the same exception safety as copy-and-swap, without using copy-and-swap?
//	Rule of 4.5:
//	LINK: https://stackoverflow.com/questions/45754226/what-is-the-rule-of-four-and-a-half#:~:text=Basically%2C%20at%20the%20end%20of,gives%20you%20strong%20exception%20guarantee).
//	<(Recognize move-assign is probably unneccessary)>
//	Extends the copy-and-swap idiom to the rule-of-5
//	copy-ctor/dtor handles resource
//	move-ctor/assignment call swap
//	friend swap is a way to declare a non-member function in the class body (best practice for swap?), calls 'swap()' for each member variable (which must either provide their own 'swap', or we fall back to 'std::swap') (note that 'std::swap' calls the copy-ctor)
//	C++ does not support implicit swap
//	<(swap is (in some ways) analogous to move-assign?)>
//	{{{
//	opinion on copy-and-swap: neat, provides exception guarentees at the cost of performance
//	}}}
//	{{{
//	Although it can save code duplication, using copy-and-swap simply results in worse classes, to be blunt. You are hurting your class' performance, including move assignment (if you use the unified assignment operator, which I'm also not a fan of), which should be very fast. In exchange, you get the strong exception guarantee, which seems nice at first. The thing is, that you can get the strong exception guarantee from any class with a simple generic function:
//	template <class T>
//	void copy_and_swap(T& target, T source) {
//	    using std::swap;
//	    swap(target, std::move(source));
//	}
//	And that's it. So people who need strong exception safety can get it anyway. And frankly, strong exception safety is quite a niche anyhow.
//	The real way to save code duplication is through the Rule of Zero: choose member variables so that you don't need to write any of the special functions. In real life, I'd say that 90+ % of the time I see special member functions, they could have easily been avoided. Even if your class does indeed have some kind of special logic required for a special member function, you are usually better off pushing it down into a member. Your logger class may need to flush a buffer in its destructor, but that's not a reason to write a destructor: write a small buffer class that handles the flushing and have that as a member of your logger. Loggers potentially have all kinds of other resources that can get handled automatically and you want to let the compiler automatically generate copy/move/destruct code.
//	The thing about C++ is that automatic generation of special functions is all or nothing, per function. That is the copy constructor (e.g.) either gets generated automatically, taking into account all members, or you have to write (and worse, maintain) it all by hand. So it strongly pushes you to an approach of pushing things downwards.
//	In cases where you are writing a class to manage a resource and need to deal with this, it should typically be: a) relatively small, and b) relatively generic/reusable. The former means that a bit of duplicated code isn't a big deal, and the latter means that you probably don't want to leave performance on the table.
//	In sum I strongly discourage using copy and swap, and using unified assignment operators. Try to follow the Rule of Zero, if you can't, follow the Rule of Five. Write swap only if you can make it faster than the generic swap (which does 3 moves), but usually you needn't bother.
//	}}}
//	{{{
//	Rule of four and a half: Same as Rule of 5 but with copy and swap idiom. With the inclusion of the swap method, the copy assignment and move assignment merge into one assignment operator.
//	Destructor, Copy constructor, move constructor, assignment, swap (the half part)
//	Destructor: ~Class();
//	Copy constructor: Class(Class &);
//	Move constructor: Class(Class &&);
//	Assignment: Class & operator = (Class);
//	Swap: void swap(Class &);
//	There are no warnings, the advantage is that it is faster in assignment as a pass by value copy is actually more efficient than creating a temporary object in the body of the method.
//	And now that we have that temporary object, we simply perform a swap on the temporary object. It's automatically destroyed when it goes out of scope and we now have the value from the right-hand side of the operator in our object.
//	}}}
class Rock {
	int* x;
public:
	~Rock() { delete x; }
	Rock(int val) : x(new int(val)) { cout << "Rock(int)\n"; }
	Rock(const Rock& rhs) {
		cout << "Rock(Rock&)\n";
		//	{{{
		//	Ongoing: 2022-04-28T00:53:37AEST copy-and-swap prevents us defining ctors in terms of assigments?
		//*this = rhs;
		//Rock temp(rhs);
		//swap(*this, temp);
		//	Ongoing: 2022-04-28T00:56:32AEST can't use copy-ctor in copy-ctor ((I feel) we have <encountered/explored> this problem before(?)>)
		//Rock temp(rhs);
		////*this = rhs;
		//swap(*this, temp);
		//	Ongoing: 2022-04-28T00:57:22AEST does the rule-of-4.5 require that we implement copy-ctor not in terms of swap() (we are at this point providing two implementations?)
		//	}}}
		x = new int(*rhs.x);
	}
	Rock(Rock&& rhs) {
		cout << "Rock(Rock&&)\n";
		//*this = move(rhs);
		swap(*this, rhs);
	}
	//	Ongoing: 2022-04-28T00:48:11AEST I am told I can replace operator(Rock&)/operator(Rock&&) with operator(Rock) and have it all just work(?)
	Rock& operator=(const Rock& rhs) {
		cout << "operator=(Rock&)\n";
		Rock temp(rhs);
		swap(*this, temp);
		return *this;
	}
	//	Ongoing: 2022-04-28T00:46:14AEST reason for not providing 'operator=(Rock&&)' (according to the 4.5-idiom) (besides simply 'swap' making it redundent?) (if one calls 'swap(rock1, rock2)' does it all just work?)
	Rock& operator=(Rock&& rhs) {
		cout << "operator=(Rock&&)\n";
		swap(*this, rhs);
		return *this;
	}
	friend void swap(Rock& lhs, Rock& rhs) noexcept {
		cout << "friend swap(Rock&, Rock&)\n";
		using std::swap;
		swap(lhs.x, rhs.x);
	}
};
//	Ongoing: 2022-04-28T00:42:20AEST supposedly, one can use copy-and-swap, and one passing-by-value for assignment to handle both copy/move-assign with one function (and it all works)? [...] (begging the wider question, is moving something to a function that accepts it by-value efficent?)



int main()
{
	Fruit f1(5);

	//	equivalent: copy-ctor
	Fruit f2(f1);
	//Fruit f2 = f1;

	//	equivalent: move-ctor
	Fruit f3(move(f1));
	//Fruit f3 = move(f1);

	//	not equivalent:
	//f1(5);					//	invokes operator()(int)
	f1 = 5;						//	invokes Fruit(int) then operator=(Fruit&&)

	//	copy-assign
	f2 = f1;

	//	move-assign
	f2 = move(f1);
	cout << "\n";


	A a1;
	A a2 = a1;


	Rock r1(5);
	Rock r2(r1);
	Rock r3(move(r1));
	r1 = 5;
	r2 = r1;
	r2 = move(r1);
	cout << "\n";
	cout << "done\n";


	return 0;
}

//	Summary:
//		
