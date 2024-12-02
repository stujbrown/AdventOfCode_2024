#include "aoc_days.h"

#include <sstream>

namespace
{
    inline int incrementIfEqual(int index, int indexToSkip) 
    {
        return (indexToSkip == index) ? (index + 1) : index; 
    }

    bool checkRecord(const std::vector<int>& record, int skipIndex)
    {
        int changeDirection = 0;
        const size_t numRecordValues = (skipIndex != record.size() - 1) ? record.size() : record.size() - 1;
        for (int i = incrementIfEqual(0, skipIndex) + 1; i < numRecordValues; i = incrementIfEqual(i, skipIndex) + 1)
        {
            const int change = record[incrementIfEqual(i, skipIndex)] - record[incrementIfEqual(i - 1, skipIndex)];
            if (changeDirection == 0)
                changeDirection = (change > 0) ? 1 : ((change < 0) ? -1 : 0);

            if (changeDirection == 0 || (changeDirection > 0 && (change > 3 || change <= 0)) || (changeDirection < 0 && (change < -3 || change >= 0)))
                return false;
        }
        return true;
    }
}

void aoc::day2(std::string_view inputFilePath)
{
    size_t safeCount = 0, safeWithToleranceCount = 0;

    std::ifstream file(inputFilePath.data());
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<int> record;
        std::stringstream recordStream(line);
        for (int val = -1; recordStream >> val;)
            record.emplace_back(val);

        int isSafeForSkipIndex = -2; // -2 == not safe, -1 == safe without any skips, 0 > safe with skip at index
        for (int skipIndex = -1; skipIndex < static_cast<int>(record.size()) && isSafeForSkipIndex == -2; ++skipIndex)
        {
            if (checkRecord(record, skipIndex))
                isSafeForSkipIndex = skipIndex;
        }

        std::println("Record '{}' is {}, ({} with tolerance)", line, isSafeForSkipIndex == -1 ? "safe" : "unsafe", isSafeForSkipIndex > -2 ? "safe" : "unsafe");
        safeCount += (isSafeForSkipIndex == -1) ? 1 : 0;
        safeWithToleranceCount += (isSafeForSkipIndex > -2) ? 1 : 0;
    }

    std::println("Safe records: {}", safeCount);
    std::println("Safe records with tolerance: {}", safeWithToleranceCount);
}
