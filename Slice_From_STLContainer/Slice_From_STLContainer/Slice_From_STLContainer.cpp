// Slice_From_STLContainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Intent of this program is to extract / slice off data from STL container like string and vector based on start and end index. 
// Assuming indexes are within range.

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

template<typename T>
T getSlicedData(T &origContainer, size_t nBegin, size_t nEnd)
{
    auto nIterBegin = origContainer.begin();
    auto nIterEnd = nIterBegin + nEnd + 1; // Inclusive last one
    T sliceContainer(nIterBegin + nBegin, nIterEnd);

    return sliceContainer;
}

int main()
{
    std::string sData("123456789");
    std::string sNewData = getSlicedData(sData, 1, 5);

    // Print sliced data from string
    std::cout << sNewData << std::endl;

    std::vector<int> arr = { 1, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector<int> ans = getSlicedData(arr, 1, 5);

    // Print sliced data from string
    for (const auto& i : ans)
        std::cout << i << ' ';
    std::cout << std::endl;

    return 1;
}

