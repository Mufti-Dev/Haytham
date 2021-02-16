// EnCode_Decode_Ptr.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

void TestFn()
{
	printf("This is TestFn\n");
}

int main()
{
	typedef void (*FUNC_PTR)();
	FUNC_PTR fp = (FUNC_PTR)EncodePointer(&TestFn);

	(*fp)(); /* It stops execution here since the pointer is encoded. Call to EncodePointer() obsfucated the pointer value and any 
			 attempt to use obsfucated pointer terminates the code. A step towards protection against hijacking pointer in system. 
			 This is specific to windows and these pair of functions present in kernel32.dll. Once this line gets commented out, 
			 DecodePointer call restores pointer and function call executes */

	FUNC_PTR fp1 = (FUNC_PTR)DecodePointer(fp);
	(*fp1)();

	return 1;
}
