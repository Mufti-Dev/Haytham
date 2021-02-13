/* MS Excel column name to number mapping.
Example: Column: A is 1, B = 2, AA = 27 and so on*/

/*Author: Mufti Md*/

#include <algorithm>
#include <iostream>
#include <string>

/*This function converts Excel column name to Number*/
int getExcelWorksheetColIDfromColName(std::string sColName){
    int result = 0;
    for(auto &ch : sColName){
        result = result * 26 + ch - 'A' + 1;
    }
    
    return result;
}

/*This function gets Excel column name from Number*/
std::string getExcelColnameFromNumber(int nNum){
    std::string sResult;
    while(nNum-- > 0){
        sResult += (nNum % 26) + 'A';
        nNum /= 26;
    }
    
    std::reverse(sResult.begin(), sResult.end());
    return sResult;
}

int main() {
    // Get Number from Excel column name
    std::cout << getExcelWorksheetColIDfromColName("A") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("AA") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("Z") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("ZA") << std::endl;
    std::cout << getExcelWorksheetColIDfromColName("AAA") << std::endl;
    // Get Excel Coloumn name from Number
    std::cout << getExcelColnameFromNumber(28) << std::endl;
    std::cout << getExcelColnameFromNumber(677) << std::endl;
    std::cout << getExcelColnameFromNumber(703) << std::endl;
    return 0;
}