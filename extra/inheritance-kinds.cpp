//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	public, protected, and private inheritance:
//		public: public/protected members in Base remain public/protected in Derived
//		protected: public/protected members in Base become protected in Derived
//		private: public/protected members in Base become private in Derived
//		all: private members of the base class are inaccessible in Derived


//	LINK: https://www.programiz.com/cpp-programming/public-protected-private-inheritance
//	{{{
//	Non-public inheritance breaks the is-a relationship, (that is): 
//			Implicit conversions form Derived to Base won't work
//			static_cast-s from Base to Derived won't work

//	public inheritance: 'is-a'
//			<(Anywhere Base is accepted, Derived is accepted too?)>

//	protected inheritance: (protected) 'implemented-in-terms-of' (rarely useful)
//			<>

//	private inheritance: 'implemented-in-terms-of'
//			<Containment/composition> is usually a better solution.


//	}}}

int main()
{
	return 0;
}


