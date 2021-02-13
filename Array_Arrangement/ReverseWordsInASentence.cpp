/* This program just reverses all words in a string
Example: Input string: Life is beautiful and after reversing all words in a string shall be: beautiful is Life*/

/*Author: Mufti Md*/

#include <algorithm>
#include <iostream>
#include <string>

/* Using space complexity of O(1) and time complexity O(N) */
std::string reverseWordsInSentence(std::string str){
    std::reverse(str.begin(), str.end());
    
    int start = 0, end = 0;
    while((end = str.find(" ", start)) != std::string::npos){
        std::reverse(str.begin() + start, str.begin() + end);
        start = end + 1;
    }
    
    std::reverse(str.begin() + start, str.end());
    
    return str;
}

#include <stack>
std::string reverseWordsInSentenceByStack(std::string sSentence) {
    std::stack<std::string> stk;
    std::string s;
    for (auto& ch : sSentence) {
        if (ch != ' ') {
            s += ch;
        }
        else {
            stk.push(s);
            stk.push(" ");
            s.clear();
        }
    }
    stk.push(s);
    s.clear();

    while (stk.size() > 0) {
        s += stk.top();
        stk.pop();
    }

    return s;
}

int main() {
    std::string sInput = "Life is beautiful";
    std::cout << reverseWordsInSentence(sInput) << std::endl;
    std::cout << reverseWordsInSentenceByStack(sInput) << std::endl;
    return 0;
}