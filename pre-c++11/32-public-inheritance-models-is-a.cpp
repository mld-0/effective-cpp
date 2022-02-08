#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Default inheritance visibility level: private inheritance for a derived class, public inheritance for derived struct (regardless of whether base class is a class or struct)

//	Public, protected, and private inheritance: 
//	(protected inheritance is of dubious <use/value>) (see item <>)
//		public inheritance: public/protected members of the base class remain public/protected in derived class
//		protected inheritance: public/protected members of the base class become protected in derived class 
//		private inheritance: public/protected members of the base class become private in the derived class
//	The private members of the base class are inaccessible to the derived class

//	TODO: 2022-02-08T03:08:12AEDT effective-c++, item 32, public inheritance models 'is a', (on the use of) the word <necessarily> being necessary when describing the reverse of the public inheritance 'is a' relationship: 'If B publicly inherits from A, then B is an A (but A is not <necessarily> a B)'

//	The single most important rule in object-oriented C++:
//	Public inheritance means 'is a'
class A {};
class B: public A {};
//	If B publicly inherits from A, then B is an A (but A is not <necessarily> a B)
//	B 'is a' A

//	Student (the derived class) publicly inherits from Person (the base class):
//	An instance of Student 'is a' Person (while an instance of Person is not <necessarily> a Student). 
//	The class Person represents a more general concept than the class Student, the class Student represents a more specialized concept than the class Person.
//	<(In C++)> Anywhere a Person can be used, a Student can be used, (because a Student is a Person)
//	If a Student is required, a Person cannot <necessarily> be used (because a Person is not <necessarily> a Student)
class Person {};
class Student: public Person {};
//	Anything that is true of, or applies to, or is a property of a Person is also true of a Student
//	<when we are talking about <what even is an instance of an object on the stack> <(only (able) to pass a Student via a Person pointer ... when using pointers)>>

void eat(const Person& p) {}		//	anyone can eat
void study(const Student& s) {}		//	only students can study


//	Consider: the relationship between birds and penguins: generally we might say that birds can fly, however a penguin is a bird, and a penguin cannot fly. If we were to reflect this through 'is a' inheritance:
class Bird {
};
class FlyingBird: public Bird {
	virtual void fly();
};
class Penguin: public Bird {
};
//	An alternative solution to the problem here would be to declare the virtual 'fly()' function for 'Bird', and implement a fly function for Penguin that generates a runtime error when called. This is non-ideal, it is better to have the compiler enforce the rule "Penguins can't fly" than it is to rely on an error in this way.


//	Consider: the relationship between Square and Rectangle (specifically the question, should Square publicly inherit Rectangle?) A Rectangle may provide an interface that allows length/width to be set to different values. That is, Rectangle has behaviour that is not applicable to Square. If Square inherits this interface, it is inheriting behaviour that is not correct for a Square, and so a public inheritance 'is a' relationship is incorrect.
//	The compiler will allow inheritance relationships that are not correct.
//	<what about the case of overriding class functions (retroactively requiring the base class equivalents to be virtual)>
//	In this way, (public) (and all) inheritance oftern defies intuition. 
//	From effective-c++: "In time, the notion of having Penguin inherit from Bird or Square inherit from Rectangle will give you the same funny feeling you probably get now when somebody shows you a function several pages long. It’s possibly the right way to approach things, it’s just not very likely."


int main()
{
	Person p;
	Student s;

	//	Ongoing: 2022-02-08T03:19:49AEDT behaviour when passing by pointer (can we violate the 'is a' rule?)
	eat(p);
	eat(s);
	study(s);
	//study(p);		//	error

	return 0;
}

//	Summary:
//		Public inheritance means 'is a'. Everything that applies to the base class must also apply to the derived class, because every derived class object is a base class object.

