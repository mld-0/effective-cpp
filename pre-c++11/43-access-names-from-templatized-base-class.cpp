#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	In some sense, when moving from OOP C++ to Template C++, inheritance stops working
//	<>

class MsgInfo {
public:
	MsgInfo(string msg) : message(msg) {}
	string getMessage() const { return message; }
private:
	string message;
};

class CompanyA {
public:
	void sendClearText(string msg) { cout << "CompanyA::sendClearText, msg=(" << msg << ")\n"; }
	void sendEncText(string msg) { cout << "CompanyA::sendEnc, msg=(" << msg << ")\n"; }
};
class CompanyB {
public:
	void sendClearText(string msg) { cout << "CompanyB::sendClearText, msg=(" << msg << ")\n"; }
	void sendEncText(string msg) { cout << "CompanyB::sendEnc, msg=(" << msg << ")\n"; }
};
class CompanyZ {
public:
	void sendEncText(string msg) { cout << "CompanyZ::sendEnc, msg=(" << msg << ")\n"; }
};

template<typename CompanyT>
class MessageSender {
public:
	void sendClear(const MsgInfo& info) { 
		CompanyT c;
		string msg = info.getMessage();
		cout << "MessageSender::sendClear, msg=(" << msg << ")\n"; 
		c.sendClearText(msg);
	}
	void sendEnc(const MsgInfo& info) { 
		CompanyT c;
		string msg = info.getMessage();
		cout << "MessageSender::sendEnc, msg=(" << msg << ")\n"; 
		c.sendEncText(msg);
	}
};

//	Specialize MessageSender for CompanyZ (which does not support 'sendClearText()')
template<>
class MessageSender<CompanyZ> {
public:
	void sendEnc(const MsgInfo& info) { 
		CompanyZ c;
		string msg = info.getMessage();
		cout << "MessageSender<CompanyZ>::sendEnc, msg=(" << msg << ")\n"; 
		c.sendEncText(msg);
	}
};

//	The compiler is unable to determine what 'LoggingMessageSender_i' inherits from, since 'MessageSender<CompanyT>' is a class template, not a class: C++ recognizes that base class templates may be specialized, and these specializations may not offer the same interface as the general template. As a result is refuses to look in the base class template for inherited names.
template<typename CompanyT>
class LoggingMessageSender_Invalid: public MessageSender<CompanyT> {
public:
	void loggingSendClear(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendClear, msg=(" << info.getMessage() << ")\n";
		//sendClear(info);		//	invalid
	}
	void loggingSendEnc(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendEnc, msg=(" << info.getMessage() << ")\n";
		//sendEnc(info);		//	invalid
	}
};

//	There are three work-arounds.
//	Firstly, the calls to the base class functions may be prefaced with 'this->':
template<typename CompanyT>
class LoggingMessageSender_i: public MessageSender<CompanyT> {
public:
	void loggingSendClear(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendClear, msg=(" << info.getMessage() << ")\n";
		this->sendClear(info);
	}
	void loggingSendEnc(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendEnc, msg=(" << info.getMessage() << ")\n";
		this->sendEnc(info);
	}
};
//	Secondly, 'using' declarations can be used to bring Base class names into the Derived class scope:
template<typename CompanyT>
class LoggingMessageSender_ii: public MessageSender<CompanyT> {
public:
	//	Ongoing: 2022-02-12T20:27:16AEDT are these (using declarations) best placed in the class body, or in the class functions in which they are used? (they cannot be placed in functions?)
	using MessageSender<CompanyT>::sendClear;
	using MessageSender<CompanyT>::sendEnc;
	void loggingSendClear(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendClear, msg=(" << info.getMessage() << ")\n";
		sendClear(info);
	}
	void loggingSendEnc(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendEnc, msg=(" << info.getMessage() << ")\n";
		sendEnc(info);
	}
};
//	Thirdly, it can be explicitly specified which class the functions are in:
//	(this is the least desirable solution since it prevents any virtual function runtime binding behaviour)
template<typename CompanyT>
class LoggingMessageSender_iii: public MessageSender<CompanyT> {
public:
	//	Ongoing: 2022-02-12T20:27:16AEDT are these (using declarations) best placed in the class body, or in the class functions in which they are used?
	void loggingSendClear(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendClear, msg=(" << info.getMessage() << ")\n";
		MessageSender<CompanyT>::sendClear(info);
	}
	void loggingSendEnc(const MsgInfo& info) {
		cout << "LoggingMessageSender_i::loggingSendEnc, msg=(" << info.getMessage() << ")\n";
		MessageSender<CompanyT>::sendEnc(info);
	}
};
//	Each of these promises the compiler that any subsequent specializations of the Base class template will support the interface offered by that template. If an attempt to call an unsupported function is made in the Base class, compilation will fail.


int main()
{
	MsgInfo msg("text");

	MessageSender<CompanyZ> m1;
	//m1.sendClear(msg);							//	fails to compile
	m1.sendEnc(msg);
	cout << "\n";

	MessageSender<CompanyA> m2;
	m2.sendClear(msg);
	m2.sendEnc(msg);
	cout << "\n";

	LoggingMessageSender_i<CompanyZ> lm1;
	//lm1.loggingSendClear(msg);					//	fails to compile
	lm1.loggingSendEnc(msg);
	cout << "\n";

	//LoggingMessageSender_ii<CompanyZ> lm2;		//	fails to compile
	//lm2.loggingSendClear(msg);
	//lm2.loggingSendEnc(msg);
	//cout << "\n";

	LoggingMessageSender_iii<CompanyZ> lm3;
	//lm3.loggingSendClear(msg);					//	fails to compile
	lm3.loggingSendEnc(msg);
	cout << "\n";

	LoggingMessageSender_i<CompanyA> lm4;
	lm4.loggingSendClear(msg);
	lm4.loggingSendEnc(msg);
	cout << "\n";

	LoggingMessageSender_ii<CompanyA> lm5;
	lm5.loggingSendClear(msg);
	lm5.loggingSendEnc(msg);
	cout << "\n";

	LoggingMessageSender_iii<CompanyA> lm6;
	lm6.loggingSendClear(msg);
	lm6.loggingSendEnc(msg);
	cout << "\n";

	return 0;
}

//	Summary:
//		In a class Derived from a Base class template, there are three methods to refer to Base class names:
//			1) 	precede the name with 'this->'
//			2) 	bring the names into the Derived class with 'using' declarations
//			3)	explicitly qualify the base class of the name, i.e: 'Widget::foo()'

