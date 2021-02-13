//Reverse an integer array(sorted) without swapping elements, O(1) space used

#include <iostream>
#include <vector>

std::vector<int> rearrangeInMaxMinOrder_Oof1space(std::vector<int> str) {
    int nLen = str.size();
    if (nLen == 0) return {};

    int nEnd = nLen - 1;
    int nMaxElem = str[nEnd] + 1;

    for (int i = 0; i < nLen; ++i) {
            str[i] += (str[nEnd] % nMaxElem) * nMaxElem;
            nEnd -= 1;
    }

    for (int i = 0; i < nLen; ++i)
        str[i] = str[i] / nMaxElem;

    return str;
}

int main()
{
    std::vector<int> data{1,5,9,21};
    std::vector<int> d = rearrangeInMaxMinOrder_Oof1space(data);
    return 0;
}