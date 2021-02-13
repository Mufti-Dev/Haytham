/* Arrange a sorted array (integers are in asceinding order) in max min pair.
   Example: Original Array: 1,2,3,4,5,6,7,8 After arrangement array  must look like 8,1,7,2,6,3,5,4*/

/*Author: Mufti Md*/

#include <iostream>
#include <string>

/*Using extra space of O(N)*/
std::string rearrangeInMaxMinOrder(std::string str){
    int nLen = str.length();
    if(nLen == 0) return std::string();
    
    int nBeg = 0;
    int nEnd = nLen - 1;
    
    std::string s;
    for(int i = 0; i < nLen; ++i){
        if(i % 2 == 0)
            s += str[nEnd--];
        else
            s += str[nBeg++];
    }
    return s;
}

int main() {
    std::string sInput = "abcdefgh";
    std::cout << rearrangeInMaxMinOrder(sInput) << std::endl;
    return 0;
}