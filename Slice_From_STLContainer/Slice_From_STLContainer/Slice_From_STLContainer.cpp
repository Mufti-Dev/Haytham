// Slice_From_STLContainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Intent of this program is to extract / slice off data from STL container like string and vector based on start and end index. 
// Assuming indexes are within range.

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>

// The below version doen't work for list(std::list) as iterator for list is bidirectional hence += won't work.
// Also not very clean perse. Creating a new version of getSlicedData. Code gets much simpler.
//template<typename T>
//T getSlicedData(T &origContainer, size_t nBegin, size_t nEnd)
//{
//    auto nIterBegin = origContainer.begin();
//    auto nIterEnd = nIterBegin + nEnd + 1; // Inclusive last one
//    T sliceContainer(nIterBegin + nBegin, nIterEnd);
//
//    return sliceContainer;
//}

template<typename T>
T getSlicedData(T& origContainer, size_t nBegin, size_t nEnd)
{
    T sliceContainer(std::next(origContainer.begin(), nBegin), std::next(origContainer.begin(), nEnd+1));
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

    // Print sliced data from vector
    for (const auto& i : ans)
        std::cout << i << ' ';
    std::cout << std::endl;

    std::list<int> listOfInts({ 2, 8, 7, 5, 3, 1, 4 });
    std::list<int> list = getSlicedData(listOfInts, 1, 5);

    // Print sliced data from vector
    for (int data : list)
        std::cout << data << " ";

    std::cout << std::endl;
    return 1;
}

