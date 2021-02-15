/* This program gets two numbers from an array if sum of those two numbers are equal to a supplied value X 
    Example: Input Array: 0, -1, 2, -3, 1 and sum = -2 */

/*Author: Mufti Md*/

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>


/*Sorting and two pointers technique 
std::sort complexity O(NlogN) and Space O(1)*/
void printTwoElementsSumEqualtoSumX(std::vector<int> vArr, int sum){
    // sort the array
    std::sort(vArr.begin(), vArr.end());
    
    int nLen = vArr.size();
    int nBegin = 0;
    int nEnd = nLen - 1;
    bool bFound = false;
    
    while(nBegin < nEnd){
        if(vArr[nBegin] + vArr[nEnd] == sum) {
            bFound = true;
            std::cout << vArr[nBegin] << " " << vArr[nEnd] << std::endl;
            break;
        } else if(vArr[nBegin] + vArr[nEnd] < sum){
            nBegin++;
        }
        else nEnd--;
    }
    if(!bFound) std::cout << "Not Found..." << std::endl;
}

/* Using hashing technique: Time & space Complexity O(N) */

void printTwoElementsSumEqualtoSumX_Hash(std::vector<int> v, int sum){
    std::unordered_set<int> st;
    
    for(auto& iVal:v){
        int nVal = sum - iVal;
        if(st.find(nVal) != st.end())
        {
            std::cout << iVal << " " << nVal << std::endl;
        }
        st.insert(iVal);
    }
}

int main() {
    std::vector<int> v{ 1, 4, 45, 6, 10, -8 };
    int sum = 37;
    printTwoElementsSumEqualtoSumX(v, sum);
    printTwoElementsSumEqualtoSumX_Hash(v, sum);
    return 0;
}