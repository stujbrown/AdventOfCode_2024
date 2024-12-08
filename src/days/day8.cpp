#include "aoc_days.h"

#include <vector>
#include <ranges>
#include <map>
#include <set>

namespace
{
    struct Coord { int x = -1, y = -1; };
    bool operator<(const Coord& lhs, const Coord& rhs) { return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y); };
    inline bool isInBounds(const Coord& c, size_t width, size_t height) { return c.x >= 0 && c.x < width && c.y >= 0 && c.y < height; };
}

void aoc::day8(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());
    const std::string fileContents = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const std::vector<std::string> grid = std::ranges::to<std::vector<std::string>>(fileContents | std::views::split('\n')
        | std::views::filter([](const auto& x) { return std::distance(x.begin(), x.end()) > 0; }));

    // Record all node positions first
    std::multimap<char, Coord> frequencyPositions;
    for (int y = 0; y < grid.size(); ++y)
    {
        for (int x = 0; x < grid[0].length(); ++x)
        {
            if (const char val = grid[y][x]; val != '.')
            {
                frequencyPositions.insert(std::make_pair(val, Coord{ x, y }));
            }
        }
    }

    std::set<Coord> antinodeLocations;
    for (auto frequencyItr = frequencyPositions.begin(); frequencyItr != frequencyPositions.end(); )
    {
        const char frequency = frequencyItr->first;
        auto firstItr = frequencyItr;
        for (; firstItr != frequencyPositions.end() && firstItr->first == frequency; ++firstItr)
        {
            for (auto secondItr = frequencyItr; secondItr != frequencyPositions.end() && secondItr->first == frequency; ++secondItr)
            {
                if (firstItr != secondItr)
                {
                    const Coord diff = { secondItr->second.x - firstItr->second.x, secondItr->second.y - firstItr->second.y };
                    // The other antinode is done from the other side once iterated operands swap over
                    const Coord antinode = { firstItr->second.x - diff.x, firstItr->second.y - diff.y };

                    if (isInBounds(antinode, grid[0].length(), grid.size()))
                    {
                        antinodeLocations.emplace(antinode);
                    }
                }
            }
        }
        frequencyItr = firstItr;
    }

    std::println("Num antinode locations: {}", antinodeLocations.size());
}
