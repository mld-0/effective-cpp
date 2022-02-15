//	VIM SETTINGS: {{{3
//	vim: set tabstop=4 modeline modelines=10 foldmethod=marker:
//	vim: set foldlevel=2 foldcolumn=2:
//	}}}1
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//	{{{2

//	Ongoing: 2022-02-16T00:47:00AEDT guarantees made by default 'new' (regarding alignment)?

//	Common reasons to replace the new/delete operators:
//		1) Detect usage errors. Calling delete more than once on a single piece of new-ed memory is undefined behaviour (the worst kind of bad for sometimes not being a problem). A custom new/delete pair can track allocated addresses, and can detect such multiple delete errors. A custom new/delete can also be used to place custom byte pattern 'signatures' between allocated blocks, in order to detect data underruns/overruns (writing before/after allocated block).
//		2) Improve efficency. Default implementations of new/delete are a compromise designed to support a wide variety of use cases. Therefore for some situations, a custom implementation may be a substantial speed and/or memory-usage improvement. <(What would that even look like? Implementing 'new' not-in-terms-of the default 'new'?)>
//		3) Collect usage statistics. A custom logging new/delete can provide details on how an application is using memory. <(things that can't be collected with a debugging tool?)>
//		4) To cluster related objects near each other.

//	Many architectures require that data of particular types be placed in memory at addresses that are multiples of four, or eight, whereas others do not require it but will be faster where these rules are followed. 
//	C++ requires that 'new' return pointers that are suitably alligned for any data type <(does that mean aligned for the type used, or any type regardless of the type used? ... malloc doesn't take a type?)> (as does 'malloc'). A custom new returning a pointer from 'new' or 'malloc' adhears to this requirement, however as soon as we do something like return an offset from this pointer, we may be in violation of the alignment rule for our system. This problem highlights the difficulties of custom memory management. (Instead, existing alternative memory managers may be an option).

int main()
{

	//	Note that calling delete doesn't nullify the pointer, it still contains the address of the now released memory (for this reason a pointer should be set to nullptr when deleted?) (this still doesn't address of the problem where there are multiple pointers).
	int* a1 = new int;
	cout << a1 << "\n";
	delete a1; 
	a1 = nullptr;
	cout << a1 << "\n";

	//	Alignment of 'new'-ed variables and arrays
	char* pchar1 = new char;
	char* pchar2 = new char;
	int* pint1 = new int;
	int* pint2 = new int;
	double* pdouble1 = new double;
	double* pdouble2 = new double;
	//	Ongoing: 2022-02-15T23:06:27AEDT why one must cast 'char*' to 'void*' to print a char pointer?
	cout << "pchar2-pchar1=(" << pchar2-pchar1 << ")\n";
	cout << "pchar1=(" << (void*) pchar1 << ")\n";
	cout << "pchar2=(" << (void*) pchar2 << ")\n";
	cout << "\n";
	cout << "pint2-pint1=(" << pint2-pint1 << ")\n";
	cout << "pint1=(" << pint1 << ")\n";
	cout << "pint2=(" << pint2 << ")\n";
	cout << "\n";
	cout << "pdouble2-pdouble1=(" << pdouble2-pdouble1 << ")\n";
	cout << "pdouble1=(" << pdouble1 << ")\n";
	cout << "pdouble2=(" << pdouble2 << ")\n";
	cout << "\n";
	char* pAchar1 = new char[10];
	int* pAint1 = new int[10];
	double* pAdouble1 = new double[10];
	cout << "&pAchar1[1]-&pAchar[0]=(" << &pAchar1[1]-&pAchar1[0] << ")\n";
	cout << "&pAchar1[0]=(" << (void*) &pAchar1[0] << ")\n";
	cout << "&pAchar1[1]=(" << (void*) &pAchar1[1] << ")\n";
	cout << "\n";
	cout << "&pAint1[1]-&pAint[0]=(" << &pAint1[1]-&pAint1[0] << ")\n";
	cout << "&pAint1[0]=(" << &pAint1[0] << ")\n";
	cout << "&pAint1[1]=(" << &pAint1[1] << ")\n";
	cout << "\n";
	cout << "&pAdouble1[1]-&pAdouble[0]=(" << &pAdouble1[1]-&pAdouble1[0] << ")\n";
	cout << "&pAdouble1[0]=(" << &pAdouble1[0] << ")\n";
	cout << "&pAdouble1[1]=(" << &pAdouble1[1] << ")\n";
	cout << "\n";
	//	Output: (ARM, MacOS)
	//	{{{
	//pchar2-pchar1=(16)
	//pchar1=(0x600000878040)			//	difference of 0x10 or 16 bytes = 16 chars
	//pchar2=(0x600000878050)
	//
	//pint2-pint1=(4)
	//pint1=(0x600000878060)			//	difference of 0x10 or 16 bytes = 4 ints
	//pint2=(0x600000878070)
	//
	//pdouble2-pdouble1=(2)
	//pdouble1=(0x600000878080)			//	difference of 0x10 or 16 bytes = 2 doubles
	//pdouble2=(0x600000878090)
	//
	//&pAchar1[1]-&pAchar[0]=(1)
	//&pAchar1[0]=(0x6000008780a0)		//	difference of 0x1 or 1 byte = 1 char
	//&pAchar1[1]=(0x6000008780a1)
	//
	//&pAint1[1]-&pAint[0]=(1)
	//&pAint1[0]=(0x60000047c270)		//	difference of 0x4 or 4 bytes = 1 int
	//&pAint1[1]=(0x60000047c274)
	//
	//&pAdouble1[1]-&pAdouble[0]=(1)
	//&pAdouble1[0]=(0x60000297cd20)	//	difference of 0x8 or 8 bytes = 1 double
	//&pAdouble1[1]=(0x60000297cd28)
	//	}}}
	//	Output: (x86, Linux)
	//	{{{
	//pchar2-pchar1=(32)
	//pchar1=(0x560a16cebeb0)			//	difference of 0x20 or 32 bytes = 32 chars
	//pchar2=(0x560a16cebed0)
	//
	//pint2-pint1=(8)
	//pint1=(0x560a16cebef0)			//	difference of 0x20 or 32 bytes = 8 chars
	//pint2=(0x560a16cebf10)
	//
	//pdouble2-pdouble1=(4)
	//pdouble1=(0x560a16cebf30)			//	difference of 0x20 or 32 bytes = 4 doubles
	//pdouble2=(0x560a16cebf50)
	//
	//&pAchar1[1]-&pAchar[0]=(1)
	//&pAchar1[0]=(0x560a16cec380)		//	difference of 0x1 or 1 bytes = 1 char
	//&pAchar1[1]=(0x560a16cec381)
	//
	//&pAint1[1]-&pAint[0]=(1)
	//&pAint1[0]=(0x560a16cec3a0)		//	difference of 0x4 or 4 bytes = 1 int
	//&pAint1[1]=(0x560a16cec3a4)
	//
	//&pAdouble1[1]-&pAdouble[0]=(1)	//	difference of 0x8 or 8 bytes = 1 double
	//&pAdouble1[0]=(0x560a16cec3d0)
	//&pAdouble1[1]=(0x560a16cec3d8)
	//	}}}
	//	Ongoing: 2022-02-15T23:18:23AEDT why C++ on ARM places new-ed variables 16 bytes apart while on x86 variables are placed 32 bytes apart (what is this alignment behaviour called / reason for it being default)

	char char1;
	char char2;
	int int1;
	int int2;
	double double1;
	double double2;
	cout << "&char1-&char2=(" << (&char1)-(&char2) << ")\n";
	cout << "&char1=(" << (void*) &char1 << ")\n";
	cout << "&char2=(" << (void*) &char2 << ")\n";
	cout << "&int1-&int2=(" << (&int1)-(&int2) << ")\n";
	cout << "&int1=(" << &int1 << ")\n";
	cout << "&int2=(" << &int2 << ")\n";
	cout << "&double1-&double2=(" << &double1-&double2 << ")\n";
	cout << "&double1=(" << &double1 << ")\n";
	cout << "&double2=(" << &double2 << ")\n";
	cout << "\n";
	//	Output: (ARM, MacOS)
	//	{{{
	//&char1-&char2=(1)
	//&char1=(0x16d07de17)
	//&char2=(0x16d07de16)
	//&int1-&int2=(1)
	//&int1=(0x16d07de10)
	//&int2=(0x16d07de0c)
	//&double1-&double2=(1)
	//&double1=(0x16d07de00)
	//&double2=(0x16d07ddf8)
	//	}}}
	//	Output: (x86, Linux)
	//	{{{
	//&char1-&char2=(-1)
	//&char1=(0x7ffe40bcd2f6)
	//&char2=(0x7ffe40bcd2f7)
	//&int1-&int2=(-1)
	//&int1=(0x7ffe40bcd2f8)
	//&int2=(0x7ffe40bcd2fc)
	//&double1-&double2=(-1)
	//&double1=(0x7ffe40bcd300)
	//&double2=(0x7ffe40bcd308)
	//	}}}

	int* pAint2 = new int[1];
	int* pAint3 = new int[1];
	cout << "pAint3-pAint2=(" << pAint3-pAint2 << ")\n";
	cout << "pAint2=(" << pAint2 << ")\n";
	cout << "pAint3=(" << pAint3 << ")\n";
	cout << "\n";
	//	Output: (ARM, MacOS)
	//	{{{
	//pAint3-pAint2=(4)
	//pAint2=(0x600000d800b0)
	//pAint3=(0x600000d800c0)
	//	}}}
	//	Output: (x86, Linux)
	//	{{{
	//pAint3-pAint2=(8)
	//pAint2=(0x55792136c430)
	//pAint3=(0x55792136c450)
	//	}}}


	return 0;
}

