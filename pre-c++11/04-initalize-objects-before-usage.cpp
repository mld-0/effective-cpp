#include <iostream>
#include <vector>
using namespace std;


int main()
{
	//	the statement
	int x;
	//	in some contexts will be intialized to zero, and in others its usage leads to undefined results
	//	reading uninitalized values yields undefined behaviour. Nothing good ever comes of undefined behaviour, which can range from an outright crash to pollution of data with semi-random data

	//	the C++ rules of when initalization is guarenteed to take place are complicated (but in general)
	//	In the C parts of C++, where initalization would incur a runtime cost it is not guarenteed to take place
	//	(For example) the values of a primative array are uninitalized, the values of a std::vector are initalized (to zero for integral types)
	//	For C++ objects, the responsibility for initalization falls to the constructor
	
	//	A Constructor member initalizer list initalizes variables. Assignment in the body of the constructor assigns to them. Member initalizer lists should be prefered.
	//	Base classes are initalized before derived classes
	//	The variables in the intitalizer list are initalized in the order they are defined in the class, not the order they appear in the list (one should therefore use the same order for both)

	return 0;
}

//	in the case of two seperately compiled source files (translation units), the relative order of initalization of non-local static objects is undefined
//	(for this reason), move each non-local static object into its own function, where it is declared static, this function returning a reference to the static object it contains. The client calls these functions instead of using the variables, 
class FileSystem {
public:
	static FileSystem& tfs() {
		static FileSystem fs;  	// optionally assign value
		return fs;
	}
	int numDisks() {
		return 0;
	}
};
class Directory {
	Directory()
	{
		size_t disks = FileSystem::tfs().numDisks();
	}
	static Directory& tempDir() {
		static Directory td;	//	optionally assign value
		return td;
	}
};
//	Using 'tfs()' and 'tempDir()' instead of the 'fs' and 'td' results eliminates such initalization race conditions *in as single threaded environemnt*

//	Summary:
//		Manually initalize built-in (primative) types to eliminate undefined behaviour
//		Prefer member initalization lists to assignement in constructor body
//		Replace <non-local static objects with local static objects>


