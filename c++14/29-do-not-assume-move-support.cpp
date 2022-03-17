//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	TODO: 2022-03-17T20:09:43AEDT effective-c++, item 29, do not assume move support, what the the conditions under which an implicit move will be generated (this being a topic for item 17)
//	Support for implicit move (see item 17)
//	<>

//	When writing templates, one cannot rely on move being supported for all types.
//	There are several scenarios in which move semantics do no good:
//		The object does not support move (and is ineligable for an implicit move operator)
//		Move is not faster than copy for the object in question
//		Move is not used in some scenarios unless it is declared noexcept
//		The source object is an lvalue

//	Ongoing: 2022-03-17T20:07:20AEDT (what are the) 'very few exceptions' to only rvalues may be used as the source of a move?

//	std::array (unlike most std containers) does not offer a constant time move (although a move is faster than a copy if the type of the array supports move).

//	std::string uses the small-string-optimization, where any string smaller than 15 characters is stored within the object, not on the heap. Moving such a string is no faster than copying.


int main()
{
	return 0;
}

//	Summary:
//		When writing templates for unknown types, assume move is not present, not cheap, and not used.

