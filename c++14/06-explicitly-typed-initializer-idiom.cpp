#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	auto type deduction may make decisions contrary to our <design/needs/wants/requirements>

class Widget {};

vector<bool> features(const Widget& w) {
	return { 0, 1, 1, 1, 0, 1, 1, 1 };
}

//	Ongoing: 2022-02-21T04:42:37AEDT is the book example using 1/2 template parameter values?
void processWidget(const Widget& w, bool priority) { cout << "processWidget, priority=(" << priority << ")\n"; }

double calcEpsilon() { return 0.7; }

int main()
{
	//	Example:
	auto pw1 = make_unique<Widget>();
	unique_ptr<Widget> pw2(new Widget);

	//	Ongoing: 2022-02-21T04:14:44AEDT 'unique_ptr<Widget> pw1 = make_unique<Widget>()' vs 'unique_ptr<Widget> pw2(new Widget())'

	//	Proxy classes:
	//	vector<bool>::operator[], which returns the proxy class 'vector<bool>::reference' <(or as deciphered by YCM '__bit_reference<std::vector<bool>,true>' (and a typeid() == typeid() match between them passes))>
	//	vector<bool> is optimized for space, storing <(8 values per byte)>, since C++ does not have the language features to provide a 'bool&' representation of each bit, a proxy class is used to achieve the same functionality (see below).
	//	LINK: https://en.cppreference.com/w/cpp/container/vector_bool/reference


	//	Example: undefined behaviour from proxy classes into temporary objects
	//	The call to 'features()' returns by-value a temporary <(rvalue)> vector<bool> object, on which operator[] is applied, returning a proxy class which emulates a reference into the n-th bit of the temporary object. As soon as the assignment is completed, this temporary object is destroyed and the value of the proxy object contained by 'priority_i' is undefined. Because 'priority_ii' is converted into bool before the temporary object is destroyed, its value is correct.
	auto priority_i = features(*pw1.get())[3];				//	T = vector<bool>::reference 	(undefined)
	bool priority_ii = features(*pw2.get())[3];				//	T = bool

	//	Explicitly Typed Initializer Idiom: using 'static_cast<bool>()' to force 'auto' to deduce the correct type
	auto priority_iii = static_cast<bool>(features(*pw1.get())[3]);

	//	Our use of 'auto' has deduced 'priority_i' as vector<bool>::reference, a proxy-class simulating 'bool&', a reference into the <temporary/rvalue> instance of vector<bool> returned by 'features()', the value of which becomes undefined when the instance is destructed after the assignment. 
	//	This is an example of a proxy-class failing to transparent in implementing its functionality (proxy-classes being something that do not work well with 'auto'). Here we have undefined behaviour with neither a runtime nor compile-time error, that being the sort of error that can be the most insidious to track-down.
	processWidget(*pw1.get(), priority_i);
	processWidget(*pw2.get(), priority_ii);
	processWidget(*pw1.get(), priority_iii);

	//	<(Book advice: be familiar enough with the libraries (their documentation, or failing that their headers) one uses to be aware of such proxy classes (sage-and-practical advice), for example the odd return type of 'vector<bool>::operator[]', being a subclass 'vector<bool>::reference' -> should be a clue a proxy-class is being used)>

	vector<bool> x = {0,1,0,1,0,1,0,1};
	int x1 = 27;
	vector<int>::reference a = x1;
	(void) a;

	//	Ongoing: 2022-02-21T07:06:30AEDT optimization problem 'Matrix sum = m1 + m2 + m3 + m4' and proxy-classes (something more complex than the outline of which more than called for here?) (a 'Sum<Matrix,Matrix> object, with 'sum' being determined by a 'Sum<Sum<Sum<Matrix,Matrix>,Matrix,Matrix>' object (a detail clients should be shielded from)).

	//	Force auto to deduce a float
	auto ep = static_cast<float>(calcEpsilon());

	return 0;
}

//	Summary:
//		Invisible 'proxy-class' types can cause auto to deduce the wrong type for an initializing expression, this lead to undefined behaviour the cause of which is difficult to track down.
//		The explicitly typed initializer idiom 'auto var = static_cast<double>(x)' can force auto to deduce the correct type.

