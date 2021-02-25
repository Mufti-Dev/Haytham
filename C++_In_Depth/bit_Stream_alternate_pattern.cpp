/* This program checks if the numbers underlying bit streams are in alternate pattern
    Example: (42)000000101010 will return true and (80) will return false.*/

// Author: Mufti Md

#include <iostream>

bool patternCheck(int a)
{
	int b = a >> 1;
        /*if a=000000101010 then b=0=00000010101 rightshift of a by 1*/
	//now xor of a and b
	int n = (a^b) + 1;
	if((n & (n-1)) == 0) 
		return true;
	
	return false;
}

int main()
{
	if(patternCheck(10))
	{
		printf("Alternate Pattern...");
	}
	else
	{
		printf("No alternate Pattern...");
	}
	return 0;
}