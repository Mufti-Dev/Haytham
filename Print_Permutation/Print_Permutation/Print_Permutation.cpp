// Print_Permutation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Problem Statement: Print all permutations of a number N greater than itself
// This problem can be solved easily by using C++ 11
/* 1. Convert the input number to string by using std::to_string
   2. Then use std::next_permutation()*/

#include <algorithm>
#include <iostream>
#include <string>

void printPermutation(int N)
{
    std::string ss = std::to_string(N);

    while (std::next_permutation(ss.begin(), ss.end()))
    {
        std::cout << ss << std::endl;
    }
}

int main()
{
    int abc = 314;
    printPermutation(abc);
    return 0;
}