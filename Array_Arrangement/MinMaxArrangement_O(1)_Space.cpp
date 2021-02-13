/* Arrange a sorted array (integers are in asceinding order) in max min pair.
   Example: Original Array: 1,2,3,4,5,6,7,8 After arrangement array  must look like 8,1,7,2,6,3,5,4 */

/*Author: Mufti Md*/

#include <iostream>
#include <vector>

std::vector<int> rearrangeInMaxMinOrder_Oof1space(std::vector<int> str) {
    int nLen = str.size();
    if (nLen == 0) return {};

    int nBeg = 0;
    int nEnd = nLen - 1;
    int nMaxElem = str[nEnd] + 1;

    for (int i = 0; i < nLen; ++i) {
        if (i % 2 == 0) {
            str[i] += (str[nEnd] % nMaxElem) * nMaxElem;
            nEnd -= 1;
        }
        else {
            str[i] += (str[nBeg] % nMaxElem) * nMaxElem;
            nBeg += 1;
        }
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