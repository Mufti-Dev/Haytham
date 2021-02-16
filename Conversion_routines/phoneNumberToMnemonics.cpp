// This program takes a telephone number and returns all possible mnemonics
/* Telephone keypad has 0, 1, 2 = "abc", 3 = "def", 4 = "ghi", 5 = "jkl", 
6 = "mno", 7 = "pqrs", 8 = "tuv", 9 = "wxyz" */

/* For a digit of n digits, 4 character combination possible hence complexity of a 
telephone number of n digits is 0(n4^n) */

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> numberToMnemonics(std::string digits) {
    std::vector<std::string> res;
    std::vector<std::string> charMap = { "0", "1", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz" };
    res.emplace_back("");
    for (size_t i = 0; i < digits.size(); ++i) {
        std::vector<std::string> tempRes;
        std::string chars = charMap[digits[i] - '0'];
        
        for (size_t c = 0; c < chars.size(); ++c) {
            for (size_t j = 0; j < res.size(); ++j) {
                tempRes.emplace_back(res[j] + chars[c]);
            }
        }

        res = tempRes;
    }

    return res;
}

int main()
{
    std::vector<std::string> v = numberToMnemonics("9811234568");
    for (auto& ch : v) {
        std::cout << ch << " ";
    }
    std::cout << "\n";

    return 0;
}