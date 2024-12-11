#include "aoc_days.h"
#include <vector>
#include <numeric>
#include <map>

struct PlutoPebbleSolver
{
    size_t countStonesAfterBlinks(int64_t stone, int depth, int maxBlinks)
    {
        if (depth == maxBlinks)
            return 1;

        if (const auto itr = countsByStoneDepthPair.find({ stone, depth }); itr != countsByStoneDepthPair.end())
            return itr->second;

        size_t r = 0;
        if (stone == 0)
        {
            r = countStonesAfterBlinks(1, depth + 1, maxBlinks);
        }
        else if (const int64_t numDigits = (int64_t)floor(log10(stone) + 1); (numDigits % 2 == 0))
        {
            const int64_t p = (int64_t)std::pow(10, numDigits / 2);
            const int64_t left = stone / p;
            r = countStonesAfterBlinks(left, depth + 1, maxBlinks) + countStonesAfterBlinks(stone - (left * p), depth + 1, maxBlinks);
        }
        else
        {
            r = countStonesAfterBlinks(stone * 2024, depth + 1, maxBlinks);
        }

        countsByStoneDepthPair.emplace(std::make_pair(stone, depth), r);
        return r;
    }

    size_t solve(const std::vector<int64_t>& stones, int numBlinks)
    {
        return std::accumulate(stones.begin(), stones.end(), size_t(0), [&](size_t total, int64_t x) {
            return total + countStonesAfterBlinks(x, 0, numBlinks); });
    }

    std::map<std::pair<int64_t, int>, size_t> countsByStoneDepthPair;
};

void aoc::day11(std::string_view inputFilePath)
{
    std::vector<int64_t> stones;
    std::ifstream file(inputFilePath.data());
    for (int64_t stone; file >> stone;)
        stones.emplace_back(stone);

    std::println("Num stones after 25 blinks: {}", PlutoPebbleSolver().solve(stones, 25));
    std::println("Num stones after 75 blinks: {}", PlutoPebbleSolver().solve(stones, 75));
}
