/*This program calculates length of substring which has no repeating characters*/
// Author: Mufti Md

#include <iostream>
#include <string>
#include <map>

using namespace std;

int longestUniqueSubsttr(string str)
{
    int n = str.size();
    if (n == 0) return 0;

    map<char, int> m;
    int result = 0;
    for (int start = 0, end = 0; end < n; ++end) {
        char ch = str[end];
        auto it = m.find(ch);
        if (it != m.end()) {
            start = max(it->second+1, start);
            it->second = end;
        }
        else
            m.insert(pair<char, int>(ch, end));

        result = max(result, end - start + 1);
    }
    
    return result;
}

int main()
{
    std::string s = "abcabcbb"; // Length = 3, abc
    std::cout << longestUniqueSubsttr(s) << std::endl;
    s = "bbbbbbb"; // Only one i.e; b
    std::cout << longestUniqueSubsttr(s) << std::endl;
    s = "dvdf"; // Length = 3, vdf
    std::cout << longestUniqueSubsttr(s) << std::endl;
}