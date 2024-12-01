#include "aoc_days.h"

#include <map>

void aoc::day1(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());
    std::vector<int> leftEntries, rightEntries;
    std::map<int, int> rightEntryCounts;

    int a, b;
    while (file >> a >> b)
    {
        leftEntries.insert(std::lower_bound(leftEntries.begin(), leftEntries.end(), a, std::less<int>()), a);
        rightEntries.insert(std::lower_bound(rightEntries.begin(), rightEntries.end(), b, std::less<int>()), b);

        if (!rightEntryCounts.try_emplace(b, 1).second)
            rightEntryCounts[b] += 1;
    }

    size_t diffTotal = 0;
    size_t similarityScore = 0;
    for (int i = 0; i < leftEntries.size(); ++i)
    {
        const int left = leftEntries[i];
        const int right = rightEntries[i];

        const int diff = abs(left - right);
        diffTotal += diff;

        const auto rightCountEntry = rightEntryCounts.find(left);
        similarityScore += (rightCountEntry != rightEntryCounts.end()) ? (left * rightCountEntry->second) : 0;
    }

    std::print("Total difference: {}\n", diffTotal);
    std::print("Total similarity score: {}\n", similarityScore);
}
