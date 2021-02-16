// Convert int to string
#include <iostream>
#include <string>
#include <stack>

std::string intToString(int num){
    bool isNegative = false;
    
     if(num < 0){
        num = -num;
        isNegative = true;
    }
    //std::string s;
    std::stack<std::string> stk;
    while(num){
        std::string st;
        st += '0' + num % 10;
        stk.push(st);
        num /= 10;
    }
    std::string s;
    if(isNegative) s += '-';
    while(!stk.empty()){
        s += stk.top();
        stk.pop();
    }
   return s;
}

int main() {
    // Write C++ code here
    int num = -999999;
    std::cout << intToString(num) << std::endl;;
    
    return 0;
}