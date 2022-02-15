#include <iostream>
#include <vector>
#include <string>
using namespace std;

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


	return 0;
}

