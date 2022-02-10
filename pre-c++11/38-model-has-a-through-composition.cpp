#include <iostream>
#include <list>
#include <vector>
#include <string>
using namespace std;

//	Composition is the relationship between types when an object of one type contains objects of another type.
//	(Also known as: layering, containment, aggregation, and embedding).
//	Composition may mean either: 'has a', or 'implemented in terms of' (the former applies to the Application domain, the later to the Implementation domain)

class Address {};
class PhoneNumber {};
class Person {
public:
	//	...
private:
	string name;
	Address address;
	PhoneNumber contactNum;
};


template<typename T>
class Set {
public:
	bool member(const T& item) const {
		return find(data.begin(), data.end(), item) != data.end();
	}
	void insert(const T& item) {
		if (!member(item)) data.push_back(item);
	}
	void remove(const T& item) {
		//	(see item 42 regarding the use of 'typename')
		typename list<T>::iterator it = find(data.begin(), data.end(), item);
		if (it != data.end()) data.erase(item);
	}
	size_t size() const {
		return data.size();
	}
private:
	list<T> data;		//	Set 'is implemented in terms of' std::list
};


int main()
{
	return 0;
}

//	Summary:
//		Composition is when once class contains members of another class
//		In the application domain, composition means 'has a'
//		In the implementation domain, composition means 'is implemented in terms of'

