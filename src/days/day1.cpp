#include "aoc_days.h"

void aoc::day1(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());

    std::vector<int> left, right;

    int a, b;
    while (file >> a >> b)
    {
        left.insert(std::lower_bound(left.begin(), left.end(), a, std::less<int>()), a);
        right.insert(std::lower_bound(right.begin(), right.end(), b, std::less<int>()), b);
    }

    int diffTotal = 0;
    for (int i = 0; i < left.size(); ++i)
    {
        const int diff = abs(left[i] - right[i]);
        diffTotal += diff;
    }


    std::print("Total difference {}", diffTotal);
}
