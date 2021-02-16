/* This program converts a valid roman numbers to integer */

/*Author: Mufti Md*/

#include <iostream>
#include <unordered_map>
#include <string>

/* convertsRomanNumbersToNumbers(Integers)*/
/* If a roman literal smaller is preceded a larger one then subtract else just add*/
int getNumberFromRomanNumerals(std::string s){
    std::unordered_map<char, int> m = {{'I', 1}, {'V', 5}, {'X', 10}, 
        {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}}; 
        
    int nLen = s.length();
    int sum = 0;
    for(int i = 0; i < nLen; ++i){
        if(m[s[i+1]] <= m[s[i]]) sum += m[s[i]];
        else sum -= m[s[i]];
    }
    return sum;
}

/*Converts a number to roman numerals*/
std::string getRomanNumeralsFromInteger(int num){
    std::string result;
    struct romanLiterals_t {int value; std::string const sLiterals;};
    romanLiterals_t const roman[] = {
        1000, "M", 
         900, "CM", 
         500, "D", 
         400, "CD", 
         100, "C", 
          90, "XC", 
          50, "L", 
          40, "XL", 
          10, "X", 
           9, "IX", 
           5, "V", 
           4, "IV", 
           1, "I", 
           0, ""};
           
    for(romanLiterals_t const* pRd = roman; pRd->value > 0; ++pRd){
        while(num >= pRd->value){
            result += pRd->sLiterals;
            num -= pRd->value;
        }
    }
    
    return result;
}

int main() {
    std::string s("MCMLXXXIV");
    std::cout << getNumberFromRomanNumerals(s) << std::endl;
    std::string s1("MMMDCCCXCVI");
    std::cout << getNumberFromRomanNumerals(s1) << std::endl;
    
    // Number to roman numerals
    std::cout << getRomanNumeralsFromInteger(3896) << std::endl;
    return 0;
}