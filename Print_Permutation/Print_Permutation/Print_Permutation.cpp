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

// Permutaion of string and prints
// Example: If Input string is ABC, then out put must be ABC, ACB, BAC, BCA, CAB, CBA
void print_all_permutation_of_string(std::string &sInput)
{
    do
    {
        std::cout << sInput << "\n";
    } while (std::next_permutation(sInput.begin(), sInput.end()));
}

int main()
{
    int nData = 314;
    printPermutation(nData);

    std::cout << "String Permutations: " << std::endl;
    std::string sData("ABC");
    print_all_permutation_of_string(sData);

    return 0;
}