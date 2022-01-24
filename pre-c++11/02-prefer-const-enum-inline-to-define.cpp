#include <string>

//	prefer the compiler to the preprocessor

//	A statement line:
#define ASPECT_RATIO 1.653
//	is never seen by the compiler; it is removed and replaced by the preprocess before the compiler recieves the code

//	instead of a macro, use a constant:
const double AspectRatio = 1.653;		// upercase letters are usually reserved for macros


//	special const cases:
//		constant pointers: when declaring a const value as a pointer, both the value and the pointer should be const
const char* const authorName = "Scott Meyers";
//	(string objects are generally prefereable to C-strings in C++):
const std::string authorName_alt("Scott Meyers");

//		class-specific constants: should be a static member
class GamePlayer {
private:
	static const int NumTurns = 5;
	int scores[NumTurns];
}

