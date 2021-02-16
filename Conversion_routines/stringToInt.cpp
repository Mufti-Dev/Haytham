#include <iostream>
#include <string>

int stringToInteger(std::string sNum){
    int result = 0;
    
    for(int i = sNum[0] == '-' ? 1 : 0; i < sNum.length(); ++i){
        int digit = sNum[i] - '0';
        result = result * 10 + digit;
    }
    
    return sNum[0] == '-' ? -result : result; 
}

int main() {
    // Write C++ code here
    std::string sNum = "-999999";
    std::cout << stringToInteger(sNum) << std::endl;;
    
    return 0;
}