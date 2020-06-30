// Find_common_Elmnts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
    std::vector<int> elemSetOne{0, 9, 55, 200, 1, 92};
    std::vector<int> elemSetTwo{98, 2, 99, 1, 55, 67, 22, 31, 75, 31};

    std::sort(elemSetOne.begin(), elemSetOne.end());
    std::sort(elemSetTwo.begin(), elemSetTwo.end());

    // Taking another vector of capacity is smaller among two vectors (set). 
    // That is the max number of common can be in iwo sets
    std::vector<int> vResult(elemSetOne.size() < elemSetTwo.size() ? elemSetOne.size() : elemSetTwo.size());

    std::vector<int>::iterator it = std::set_intersection(elemSetOne.begin(), elemSetOne.end(), 
        elemSetTwo.begin(), elemSetTwo.end(), vResult.begin());

    int nElementCmn = it - vResult.begin();

    vResult.resize(nElementCmn);

    std::cout << "There are " << nElementCmn << " common elements\n";
    std::cout << "Elements are: ";
    for (auto i : vResult)
        std::cout << i << " ";

    std::cout << std::endl;

    return 0;
}

