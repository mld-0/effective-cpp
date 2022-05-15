//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
using namespace std;
//	{{{2

//	objdump: peek into (view asm of) binary
//			objdump -d a.out
//			objdump --disassemble a.out
//	windows:
//			'dumpbin /DISAM foo.exe'

//	otool:
//		otool a.out -tV


//	g++ flags:
//			-v			verbose
//			-g			source level debug <(include source in assembly?)>
//			-S			(output assembly) only run preprocess/compilation steps (produces *.s assembly file)
//			-save-temps	

//	compiler explorer:
//	LINK: https://godbolt.org/



int f(int x) { 
	return x * x; 
} 

int main()
{
	int a1 = 5;
	int a2 = f(a1);

	cout << "a1=(" << a1 << "), a2=(" << a2 << ")\n";

	return 0;
}

