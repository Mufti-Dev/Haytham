/* This program drops sign bit from a decimal number without converting to string. A kind of C/C++ librarys abs() implementation */

// Author: Mufti Md

#include <iostream>

void printAbsValue(int val)
{
	unsigned int result;
	int const mask = val >> sizeof(int) * 7;
	result = (val ^ mask) - mask;

	std::cout << "Absolute value of " << val << " is : " << result;
}

int main()
{
	printAbsValue(-990);
	return 0;
}