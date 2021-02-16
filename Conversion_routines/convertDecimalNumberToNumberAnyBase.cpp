// Decimal Base to any base of a number
#include <algorithm>
#include <iostream>
#include <string>

std::string convertBaseForDecimalNumber(int nNumber, int toBase){
    std::string sResult;
    
    while(nNumber){
        int nRes = nNumber % toBase; 
        sResult += nRes >=10 ? 'A' + nRes - 10 : '0' + nRes;
        nNumber /= toBase;
    }
    
    std::reverse(sResult.begin(), sResult.end());
    
    return sResult;
}

int main() {
    // Write C++ code here
    int sNum = 400;
    std::cout << convertBaseForDecimalNumber(sNum, 2) << std::endl;;
    
    return 0;
}