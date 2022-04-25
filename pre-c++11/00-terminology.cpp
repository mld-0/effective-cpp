#include <iostream>

//	Declarations
extern int x;							//	object declaration

std::size_t numDigits(int number);		//	function declaration
										//	size_t is a typedef of the (compiler dependent) unsigned type used for sizes

class Widget;							//	class declaration

template<typename T>					//	template declaration
class GraphNode;



//	Definitions
int x; 									// object definition
	
std::size_t numDigits(int number)		//	function defintion
{
	std::size_t digitsSoFar = 1;
	while ((number /= 10) != 0) ++ digitsSoFar;
	return digitsSoFar;
}


class Widget {								//	class definition
public:
	Widget();								//	default constructor
	~Widget();								//	default destructor
	Widget(const Widget& rhs);				//	copy constructor
	Widget(Widget&& rhs);					//	move constructor
	Widget& operator=(const Widget& rhs);	//	copy-assignment operator
	Widget& operator=(Widget&& rhs);		//	move-assignment operator
	//	...
};


template<typename T>					//	class template definition
class GraphNode {
public:
	GraphNode();
	~GraphNode();
	//	...
};


//	initialization is the process of giving an object its first value
//	objects are initialized by their constructor 

//	TODO: 2022-02-08T23:41:24AEDT effective-c++, 00-terminology, <object/primative> initialization through '=', '()', and '{}', (and how) (god is dead and none of these options works in every scenario)

int main()
{
	return 0;
}

