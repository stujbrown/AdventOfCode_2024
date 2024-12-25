#include "aoc_days.h"
#include <array>
#include <vector>

void aoc::day25(std::string_view inputFilePath)
{
    using KeyValues = std::array<int, 5>;
    std::vector<KeyValues> keys;
    std::vector<KeyValues> locks;
    KeyValues current;
    current.fill(-1);

    int row = 0;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line);)
    {
        if (line.find_first_not_of(' ') != std::string::npos)
        {
            for (int i = 0; i < 5; ++i)
                if (line[i] == '#')
                    ++current[i];
            ++row;
        }
        if (row == 7)
        {
            if (line[0] == '.')
                keys.emplace_back(current);
            else
                locks.emplace_back(current);
            current.fill(-1);
            row = 0;
        }
    }

    size_t fitCount = 0;
    for (const auto& lock : locks)
    {
        for (const auto& key : keys)
        {
            bool isFit = true;
            for (int i = 0; i < 5; ++i)
                if (lock[i] + key[i] > 5)
                    isFit = false;

            if (isFit)
                ++fitCount;
        }
    }

    std::println("Number of fits: {}", fitCount);
}
