/* MS Excel column name to number mapping.
Example: Column: A is 1, B = 2, AA = 27 and so on*/
#include <iostream>
#include <string>

int getExcelWorksheetColIDfromColName(std::string sColName){
    int result = 0;
    for(auto &ch : sColName){
        result = result * 26 + ch - 'A' + 1;
    }
    
    return result;
}

int main() {
    // Write C++ code here
    std::cout << getExcelWorksheetColIDfromColName("A") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("AA") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("Z") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("ZA") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("AAA") << std::endl;
    return 0;
}