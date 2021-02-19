// This program sorts a string (descending order) by frequency of chars
// Used priority_queue with a labda comparator to achieve this.

// Author: Mufti Md

#include <iostream>
#include <queue>
#include <string>
#include <vector>

std::string frequencySort(std::string s) {
    int count[256] = { 0 };
    for (const auto& n : s) count[n]++;

    auto compare = [](std::pair<int, int> one, std::pair<int, int> two) {
        return one.second < two.second;
    };

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(compare)> q(compare);

    for (int i = 0; i < 256; ++i) {
        if (count[i] > 0) q.emplace(i, count[i]);
    }

    std::string result;
    while (!q.empty()) {
        auto var = q.top();
        q.pop();

        result.append(var.second, var.first);
    }

    return result;
}

int main()
{
    std::string s = "tree";
    std::cout << frequencySort(s) << std::endl;

    return 0;
}