/* This program encodes/decodes RLE for a given string*/
#include <iostream>
#include <cassert>
#include <string>

/* This function decodes (RLE) string and returns original string*/
std::string decodeRLE(const std::string str) {
    int count = 0;
    std::string result;
    for (auto& ch : str) {
        if (isdigit(ch)) {
            count = count * 10 + ch - '0';
        }
        else {
            result.append(count, ch);
            count = 0;
        }
    }

    return result;
}

/* This function encodes (RLE) for input string*/
std::string encodeRLE(const std::string& str) {
    std::string result;

    for (int i = 1, count = 1; i <= str.length(); ++i) {
        if (str[i] != str[i - 1] || i == str.length()) {
            result += std::to_string(count) + str[i - 1];
            count = 1;
        }
        else {
            ++count;
        }
    }
    return result;
}

int main()
{
    std::string s("Hello World"); 
    std::string resEncode = encodeRLE(s);
    std::string resDecode = decodeRLE(resEncode);
    std::cout << resEncode<< " - " << resDecode << std::endl;
    return 0;
}