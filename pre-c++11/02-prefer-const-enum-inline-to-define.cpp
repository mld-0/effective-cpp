#include <string>

//	prefer the compiler to the preprocessor

//	A statement line:
#define ASPECT_RATIO 1.653
//	is never seen by the compiler; it is removed and replaced by the preprocess before the compiler recieves the code
//	define does not respect scope

//	instead of a macro, use a constant:
const double AspectRatio = 1.653;		// upercase letters are usually reserved for macros


//	special const cases:
//		1) constant pointers: when declaring a const value (stored) as a pointer, both the value and the pointer should be const
const char* const authorName = "Scott Meyers";
//	(in general, std::strings are preferable to C-strings in C++):
const std::string authorName_alt("Scott Meyers");

//		2) class-specific constants: should be a static member
class GamePlayer {
private:
	static const int NumTurns = 5;		//	this is a declaration (not a definition)
	int scores[NumTurns];
};
//	class static constants of primative types can be used with only this declaration (and no definition) provided their address is not taken (in which case provide a seperate definintion in the source file (see below) (since the value is provided in the declaration, a value cannot be provided in the definition)
const int GamePlayer::NumTurns;
//	this class static constant with a value provided in the declaration may not be permitted by older compilers (in which case plase the value in the definition) (this will make the value unavailable at compile time)

//	if working under this restriction, and the value of the class constant is needed at compile time, there is the 'enum hack' - taking advantage of the fact that the values of an enumerated type can be used where ints are expected
class GamePlayer_Alt {
private:
	enum { NumTurns = 5 };
	int scores[NumTurns];
};
//	this prevents clients using the variable as a pointer of reference


//	the preprocessor is sometimes used to implement function-like macros
//	these are generally probelmatic
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))
//	correct parenthesis-ation is essential for such macros to prevent undesired behaviour
//	(even without such mistakes), such macros are prone to unexpected behaviour, for example:
//int f(int x) { return x; }
//int a = 5, b = 0;
//CALL_WITH_MAX(++a, b);		//	a is incremented twice
//CALL_WITH_MAX(++a, b+10);		//	a is incremented once

//	a better alternative to such a macro is a template inline function
template<typename T>
inline void callWithMax(const T& a, const T& b)
{
	f(a > b ? a: b);
}

//	Summary:
//		For simple constants, prefer const objects or enums to #defines
//		For function like macros, prefer inline functions to #defines




