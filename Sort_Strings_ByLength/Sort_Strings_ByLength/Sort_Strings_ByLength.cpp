// Sort_Strings_ByLength.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <string>

template <typename T, size_t array_elements>
int getElements(const T(&array)[array_elements])
{
    return array_elements;
}

int main()
{
    std::string sArray[] = {"human", "I", "am"};
    int nElem = getElements(sArray);

    std::sort(sArray, sArray + nElem, [](const std::string& lStr, const std::string& rStr) 
        { return lStr.size() < rStr.size(); });

    std::copy(sArray, sArray + nElem, std::ostream_iterator<std::string>(std::cout, " "));
    return 0;
}
