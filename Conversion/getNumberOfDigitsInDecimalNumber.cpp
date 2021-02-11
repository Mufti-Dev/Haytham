// Get number of digits in a decimal number.
#include <iostream>
#include <cmath>

int getNumberOfDigits(int nNum, int nBase){
    int result = floor(log(nNum) / log(nBase)) + 1;
    
    return result;
}

int main() {
    // Write C++ code here
    int sNum = 2987654;
    std::cout << getNumberOfDigits(sNum, 10) << std::endl;
    
    return 0;
}