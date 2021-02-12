// Get / Extract Integer part and decimal part of a number.
#include <iostream>
#include <cmath>

double getDecimalPart(double f){
    double nIntPart;
    return modf(f, &nIntPart);
}

int getIntegerPart(double f){
    double nIntPart;
    double decimalPart = modf(f, &nIntPart);
    return nIntPart;
}

int main() {
    // Write C++ code here
    double aNum = 2147.483647;
    std::cout << getDecimalPart(aNum) << std::endl;
    std::cout << getIntegerPart(aNum) << std::endl;
    return 0;
}