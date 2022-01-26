#include <iostream>
#include <vector>
#include <string>
using namespace std;

//	Many APIs require access to the underlying raw resource that may be being managed by a class
//	For this reason it can be necessary to make the underlying resource available, e.g: smart pointers offer 'get()' 

//	This problem can be handled with explicit or implicit conversion

//	Using the example of the smart pointer unique_ptr/shared_ptr:
//		Explicit conversion is done with the function 'get()'
//		Implicit conversion is made available by overloading the (dereferencing) '->' and '*' operators

//	Implicit conversion can also be achieved by defining an implicit conversion function. This can be a source of errors due to unintended type conversions


//	resource
class FontHandle {};

//	API which uses resource
FontHandle getFont() { return FontHandle(); }
void releaseFont(FontHandle fh) {}
void changeFontSize(FontHandle f, int newSize) {}

//	resource manager
class Font {
public:
	explicit Font(FontHandle fh) 
		: f(fh)
	{}
	~Font() { releaseFont(f); }

	Font(const Font& rhs) = delete;					//	disable copying
	Font& operator=(const Font& rhs) = delete;

	operator FontHandle() const 					//	implicit conversion function
	{ return f; }

	FontHandle get() const							//	explicit conversion function
	{ return f; }

private:
	FontHandle f;
};

int main()
{
	Font f(getFont());

	changeFontSize(f.get(), 12);		//	call API using explicit conversion
	changeFontSize(f, 12);				//	call API using implicit conversion (Font->FontHandle)

	FontHandle f2 = f;					//	ambigious implicit conversions can be a source of errors

	return 0;
}

//	Summary:
//		resource managing classes may need to offer access to their underlying resource
//		explicit converison (i.e: 'get()') is safer
//		implicit converison (i.e: 'operator FontHandle() const') can be more convinent but also a source of errors

