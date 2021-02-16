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

int main() {
    std::string s("MCMLXXXIV");
    std::cout << getNumberFromRomanNumerals(s) << std::endl;
    std::string s1("MMMDCCCLXXXXVI");
    std::cout << getNumberFromRomanNumerals(s1) << std::endl;
    return 0;
}