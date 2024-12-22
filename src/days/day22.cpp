#include "aoc_days.h"
#include <map>
#include <array>
#include <vector>
#include <set>

namespace
{
    int64_t solve(int64_t number)
    {
        for (int64_t i = 0; i < 2000; ++i)
        {
            int64_t operand = number * 64;
            number = number ^ operand;
            number = number % 16777216;


            operand = number / 32;
            number = number ^ operand;
            number = number % 16777216;


            operand = number * 2048;
            number = number ^ operand;
            number = number % 16777216;
        }
        return number;
    }

    int64_t solve2(int64_t number, std::map<std::array<int, 4>, size_t>& sequenceRecords)
    {
        std::array<int, 4> changeHistory;
        std::set<std::array<int, 4>> visitedThisTrader;

        int previousPrice = -1;
        for (int64_t i = 0; i < 2000; ++i)
        {
            int64_t operand = number * 64;
            number = number ^ operand;
            number = number % 16777216;


            operand = number / 32;
            number = number ^ operand;
            number = number % 16777216;


            operand = number * 2048;
            number = number ^ operand;
            number = number % 16777216;


            const int price = number % 10;
            if (i != 0)
            {
                const int change = price - previousPrice;
                memmove(changeHistory.data(), changeHistory.data() + 1, sizeof(int) * 3);
                changeHistory[3] = change;
                if (i > 3)
                {
                    if (!visitedThisTrader.contains(changeHistory))
                    {
                        auto recordItr = sequenceRecords.find(changeHistory);
                        if (recordItr == sequenceRecords.end())
                            recordItr = sequenceRecords.emplace(std::make_pair(changeHistory, 0)).first;

                        recordItr->second += price;

                        visitedThisTrader.emplace(changeHistory);
                    }
                }
            }
            previousPrice = price;

        }
        return number;
    }
}

void aoc::day22(std::string_view inputFilePath)
{
    size_t secretSum = 0;
    std::ifstream file(inputFilePath.data());

    std::map<std::array<int, 4>, size_t> sequenceRecords;
    for (int64_t secretNumber = 0; file >> secretNumber;)
    {
        secretSum += (size_t)solve(secretNumber);
        solve2(secretNumber, sequenceRecords);
    }

    size_t highestTotal = 0;
    for (const auto& entry : sequenceRecords)
    {
        if (entry.second > highestTotal)
            highestTotal = entry.second;
    }

    std::println("Secret sum: {}", secretSum);
    std::println("Most bananas you can get: {}", highestTotal);
}
