/* This program sorts roman numerals based on their values (Non lexicographically)*/

/*Author: Mufti Md*/

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>

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

/* Sort roman numerals based on corresponding integer values and then
stores roman numerals to a separate vector and return*/

std::vector<std::string> sortRomanNumerals(std::vector<std::string> sRomanNumerals){
    // A vector of pairs for roman numerals with corresponding integer
    std::vector<std::pair<int, std::string> >vectorPairs;
    for(auto& str:sRomanNumerals){
        vectorPairs.emplace_back(make_pair(
            getNumberFromRomanNumerals(str), str));
    }
    // C++ std::sort to sort based on integer values correspond to roman
    // numerals
    std::sort(vectorPairs.begin(), vectorPairs.end());
    
    // Store sorted roman numerals to a vector of strings and returned
    std::vector<std::string> result;
    for(int i = 0; i < vectorPairs.size(); ++i){
        result.emplace_back(vectorPairs[i].second);
    }
    
    return result;
}

int main() {
    std::vector<std::string> v{ "MCMIV", "MIV", "MCM", "MMIV" };
    std::vector<std::string> res = sortRomanNumerals(v);
    for(auto& val:res) std::cout << val << " ";
    std::cout << "\n";
    return 0;
}