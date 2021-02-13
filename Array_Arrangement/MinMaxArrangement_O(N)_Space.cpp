/* Arrange a sorted array (chars are in asceinding order) in max min pair.
   Example: Original Array: a,b,c,d,e,f,g,h After arrangement array  must look like h,a,g,b,6,f,c,e,d*/

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