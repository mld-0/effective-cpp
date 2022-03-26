//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	<C++98> Requirements to override a member function:
//		Base class function must be virtual
//		Base/Derived function names must be identical (except for dtors)
//		Parameter types of Base/Derived functions must be identical
//		const-ness and exception-specification of Base/Derived functions must be identical
//		return types of Base/Derived functions must be identical

//	<C++11> adds the requirement:
//		Base/Derived functions reference qualifiers must be identical

//	These requirements leave ample oppertunity to make a mistake when overriding a function. Furthermore, one should not rely on recieving a compiler warning for such errors. Declaring overriding Derived member functions 'override' reduces the chances of any such mistake not producing an error.
class Base {
public:
	virtual void mf1() const 	{ cout << "Base::mf1\n"; }
	virtual void mf2(int) 		{ cout << "Base::mf2\n"; }
	virtual void mf3() & 		{ cout << "Base::mf3\n"; }
	virtual void mf4() const 	{ cout << "Base::mf4\n"; }
};
class Derived: public Base {
public:
	virtual void mf1() const override 	{ cout << "Derived::mf1\n"; }
	virtual void mf2(int) override 		{ cout << "Derived::mf2\n"; }
	virtual void mf3() & override 		{ cout << "Derived::mf3\n"; }
	virtual void mf4() const override 	{ cout << "Derived::mf4\n"; }
};

//	Example: Member function reference qualifiers
//	{{{
class ExampleReferenceQualifiers {
public:
	vector<int> data() & {					//	invoked when '*this' is an lvalue
		return this->values;
	}
	vector<int>&& data() && {				//	invoked when '*this' is an rvalue
		return std::move(this->values);
	}
private:
	vector<int> values;
};
//	}}}

int main()
{
	return 0;
}

//	Summary:
//		Declare overriding functions 'override'
//		Member function reference qualifiers &/&& make it possible to treat lvalue/rvalue objects '*this' differently

