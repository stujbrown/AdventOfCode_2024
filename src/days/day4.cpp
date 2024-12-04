#include "aoc_days.h"

#include <ranges>
#include <array>

namespace
{
    struct Coord { int x = 0; int y = 0; };
    const std::array<Coord, 8> evaluationIncrements
    {
        Coord{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }
    };
}

void aoc::day4(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());
    const std::string fileContents = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const std::vector<std::string> grid = std::ranges::to<std::vector<std::string>>(fileContents | std::views::split('\n'));

    int matchCount = 0;
    for (int y = 0; y < grid.size(); ++y)
    {
        for (int x = 0; x < grid[y].length(); ++x)
        {
            for (const Coord& increment : evaluationIncrements)
            {
                bool matchFailed = false;
                for (int wordIndex = 0; (wordIndex < 4); ++wordIndex)
                {
                    const int nextX = x + (increment.x * wordIndex);
                    const int nextY = y + (increment.y * wordIndex);
                    if ((nextX < 0 || nextX >= grid[y].length()) || (nextY < 0 || nextY >= grid.size())
                        || grid[nextY][nextX] != "XMAS"[wordIndex])
                    {
                        matchFailed = true;
                        break;
                    }
                }

                if (!matchFailed)
                    ++matchCount;
            }
        }

        std::println("{}", grid[y].c_str());
    }

    std::println("Total matches: {}", matchCount);
}
