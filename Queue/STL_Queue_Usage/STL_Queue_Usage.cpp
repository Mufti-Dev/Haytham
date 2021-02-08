// STL_Queue_Usage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// This program generates binary number 1 to n and returns all binary ranges between number 
// [1...n] as a vector of strings

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

vector<string> findBin(int n) {
    vector<string> v(n, "0");
    queue<int> q;
    q.push(1);

    string s1, s2;
    for (int i = 0; i < n; ++i) {
        if (i > 0) 
            q.pop();

        v[i] = to_string(q.front());
        s1 = v[i] + "0";
        s2 = v[i] + "1";

        q.push(stoi(s1));
        q.push(stoi(s2));
    }

    return v;
}

int main()
{
    int n = 8;
    vector<string> v = findBin(n);

    // Diaplay all binary numbers in the range [1...n] stored in the vector
    std::copy(v.begin(), v.end(), std::ostream_iterator<string>(std::cout, " "));

    return 1;
}
