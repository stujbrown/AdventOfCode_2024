#include "aoc_days.h"

#include <ranges>
#include <array>
#include <set>

namespace
{
    using Coord = std::pair<int, int>;
    const std::array<Coord, 8> standardEvalIncrements
    {
        Coord{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }
    };
}

void aoc::day4(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());
    const std::string fileContents = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const std::vector<std::string> grid = std::ranges::to<std::vector<std::string>>(fileContents | std::views::split('\n'));

    std::set<Coord> matchedCentrepoints; // 'A' positions for each MAS match

    int standardMatchCount = 0, crossoverMatchCount = 0;
    for (int y = 0; y < grid.size(); ++y)
    {
        for (int x = 0; x < grid[y].length(); ++x)
        {
            for (const Coord& increment : standardEvalIncrements)
            {
                bool isFullMatch = true, isMasMatch = true;
                for (int wordIndex = 0; wordIndex < 4; ++wordIndex)
                {
                    const int nextX = x + (increment.first * wordIndex);
                    const int nextY = y + (increment.second * wordIndex);

                    if ((nextX < 0 || nextX >= grid[y].length()) || (nextY < 0 || nextY >= grid.size()) ||
                        grid[nextY][nextX] != "SAMX"[wordIndex])  // Match backwards for the optional 'X' on the MAS matching
                    {
                        isFullMatch = false;

                        if (wordIndex != 3)
                            isMasMatch = false;
                    }
                }

                if (isFullMatch)
                    ++standardMatchCount;

                if (isMasMatch && (increment.first == -1 || increment.first == 1) && (increment.second == -1 || increment.second == 1))
                {
                    const auto matchedCentre = Coord{ x + increment.first, y + increment.second };
                    if (matchedCentrepoints.contains(matchedCentre))
                        ++crossoverMatchCount; // Count 'A' position dupes

                    matchedCentrepoints.emplace(matchedCentre);
                }
            }
        }
    }

    std::println("Total standard matches: {}", standardMatchCount);
    std::println("Total crossover matches: {}", crossoverMatchCount);
}
