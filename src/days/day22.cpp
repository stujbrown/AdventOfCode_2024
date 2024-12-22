#include "aoc_days.h"
#include <map>
#include <array>
#include <set>

namespace
{
    int64_t solve(int64_t number, std::map<uint32_t, size_t>& sequenceRecords)
    {
        std::array<int16_t, 4> changeHistory;
        std::set<uint32_t> visitedThisTrader;

        int16_t previousPrice = -1;
        for (int64_t i = 0; i < 2000; ++i)
        {
            number = (number ^ (number * 64)) % 16777216;
            number = (number ^ (number / 32)) % 16777216;
            number = (number ^ (number * 2048)) % 16777216;

            const int16_t price = number % 10;
            if (i != 0)
            {
                const int16_t change = price - previousPrice;
                memmove(changeHistory.data(), changeHistory.data() + 1, sizeof(int16_t) * 3);
                changeHistory[3] = change;
                if (i > 3)
                {
                    const uint32_t changeHash = (uint32_t)((changeHistory[0] + 18) | ((changeHistory[1] + 18) << 5) | ((changeHistory[2] + 18) << (5 * 2)) | (changeHistory[3] + 18) << (5 * 3));
                    if (!visitedThisTrader.contains(changeHash))
                    {
                        auto recordItr = sequenceRecords.find(changeHash);
                        if (recordItr == sequenceRecords.end())
                            recordItr = sequenceRecords.emplace(std::make_pair(changeHash, 0)).first;

                        recordItr->second += price;
                        visitedThisTrader.emplace(changeHash);
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

    std::map<uint32_t, size_t> sequenceRecords;
    for (int64_t secretNumber = 0; file >> secretNumber;)
    {
        secretSum += (size_t)solve(secretNumber, sequenceRecords);
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
