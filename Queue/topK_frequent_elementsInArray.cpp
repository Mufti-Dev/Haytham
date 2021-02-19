/*This program returns top K frequent elements from an array of numbers. 
Example: [1,1,1,2,3,3,3,3,4] and K = 2. The program must return an array [3,1] where 3 is the most 
frequent element followed by 1. Assuming K is always valid i.e, 1 <= K <= length of array*/

// Author: Mufti Md

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

std::vector<int> topKFrequent(std::vector<int>& nums, int K) {

    std::unordered_map<int, int> uMap;
    for (auto& num : nums) {
        uMap[num]++;
    }

    auto compare = [](std::pair<int, int>a, std::pair<int, int>b) {
        return a.second < b.second;
    };

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(compare)> q(compare);

    for (auto m : uMap) {
        q.emplace(m.first, m.second);
    }

    std::vector<int> result;
    for (int i = 0; i < K; ++i) {
        auto var = q.top();
        q.pop();
        result.emplace_back(var.first);
    }

    return result;
}

int main()
{
    std::vector<int> v{ 1,1,8,8,8,9 };
    int K = 2;
    std::vector<int> result = topKFrequent(v, K);

    return 1;
}