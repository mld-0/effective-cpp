#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	LINK: https://stackoverflow.com/questions/3979766/how-do-traits-classes-work-and-what-do-they-do

//	Type traits are manually defined for each type. 
//	Think of a trait as a small object whose main purpouse is to carry information used by another object or algorithm to determine implementation details.
//	For example, 'iterator_traits' provides typedefs (e.g: 'value_type') for iterators

//	Using them, we can write:
iterator_traits<vector<int>::iterator>::value_type x;		//	declare int 'x'
iterator_traits<int*>::value_type y;						//	declare int 'y'

//	To make this work, there is an explicit definition <(somewhere in the iterator header)>, which reads like:
//template<typename T>
//struct iterator_traits<T*> {
//	typedef T value_type;
//	//	...
//};
//	This is a partial specialization of the 'iterator_traits' type for types of the form 'T*'
//	'iterator_traits' <is/are> specialized for other iterators, e.g: 'typename vector<T>::iterator'


int main()
{
	return 0;
}

