// Sort_Suffle_In_Part.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <vector>

/*This program is an adaptation from IIT Bombay's CS 201.x course.*/

template <typename T>
void Sort_Random_Sort(T& sData)
{
    int nPart = sData.size() / 3;
    // 1. First (size/3) elements arranged in descending order
    // 2. Next (size/3) elements shuffled randomly
    // 3. Rest of elements again arranged in descendfing order

    // 1. Using std::sort to sort in descending order
    std::sort(sData.begin(), std::next(sData.begin(), nPart), std::greater<int>());

    // 2. Using std::random_shuffle
    std::random_shuffle(std::next(sData.begin(), nPart), std::next(sData.begin(), (nPart*2)));

    // 3. Using std::sort to sort in descending order
    std::sort(std::next(sData.begin(), (nPart * 2)), sData.end(), std::greater<int>());

    // Modified vector contains: 5,4,3,2,1,<random order for 6-10>,15,14,13,12,11
}

int main()
{
    std::vector<int> sData{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    Sort_Random_Sort(sData);

    // Print altered vector elements
    for (auto i : sData)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    return 0;
}