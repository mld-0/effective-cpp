//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2
//	Ongoing: 2022-05-03T21:54:21AEST applicable to (lambdas) (and/or other things) (if only lambdas, put 'lambda' in filename (before commiting))

//	Sometimes it is neccessary to move an object into a closure. 

//	init capture: <C++14> allows an object to be moved into an 'auto' variable available to the lambda.
//	Allows us to specify: the name of a data member (in the closure class), and the expression initalizing that member.
//	<pre-C++14> such a lambda <can/must> be created manually as a class (see 'IsValidWidget') <(or 'old-init-capture emulation')>

struct Widget {
	bool status = false;
	bool isValidated() { return status; }
	bool isProcessed() { return status; }
	bool isArchived() { return status; }
};

int main() 
{

	//	Example: move unique_ptr into a closure
	auto pw = make_unique<Widget>();
	pw->status = true;
	auto func_i = [pw = move(pw) ]() { return pw->isValidated() && pw->isArchived(); };
	func_i();


	//	Example: If 'Widget' does not need to be set up, it can be created into the lambda
	auto func_ii = [pw = make_unique<Widget>()]() { return pw->isValidated() && pw->isArchived(); };
	func_ii();


	//	Equivalent class to 'func_ii'
	struct IsValidWidget {
		using DataType = unique_ptr<Widget>;
		explicit IsValidWidget(DataType&& ptr) : ptr(move(ptr)) {}
		auto operator()() const { return ptr->isValidated() && ptr->isArchived(); }
	private:
		DataType ptr;
	};
	auto func_iii = IsValidWidget(make_unique<Widget>());
	func_iii();


	//	Ongoing: 2022-05-07T04:41:02AEST (about (example?)) 'std::bind'
	//	<('Data' is a const lvalue reference, to (something that is move-constructed) (and is a member of 'func_iv'?))>
	//	Example: old-init-capture emulation
	vector<double> data;
	auto func_iv = std::bind(
			[](const vector<double>& Data) 
			{ /* use Data */ }, 
			std::move(data)
		);
	//	<(About old-init-capture emulation)>

	return 0;
}

//	Summary:
//		

