#include "aoc_days.h"
#include "vec2.h"

#include <vector>
#include <ranges>
#include <map>
#include <set>
#include <numeric>

void aoc::day8(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());
    const std::string fileContents = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const std::vector<std::string> grid = std::ranges::to<std::vector<std::string>>(fileContents | std::views::split('\n')
        | std::views::filter([](const auto& x) { return std::distance(x.begin(), x.end()) > 0; }));

    std::map<char, std::vector<Vec2>> frequencyPositions;
    for (int y = 0; y < grid.size(); ++y)
    {
        for (int x = 0; x < grid[0].length(); ++x)
        {
            if (const char val = grid[y][x]; val != '.')
            {
                const auto insertResult = frequencyPositions.insert(std::make_pair(val, std::vector<Vec2>{ Vec2{ x, y }}));
                if (!insertResult.second)
                    insertResult.first->second.push_back(Vec2{ x, y });
            }
        }
    }

    auto isInBounds = [&grid](const Vec2 & c) { return c.x >= 0 && c.x < grid[0].length() && c.y >= 0 && c.y < grid.size(); };

    std::set<Vec2> priamryAntinodeLocations;
    std::set<Vec2> allAntinodeLocations;
    for (auto itr = frequencyPositions.begin(); itr != frequencyPositions.end(); ++itr)
    {
        const std::vector<Vec2>& entries = itr->second;
        for (size_t leftIndex = 0; leftIndex < entries.size(); ++leftIndex)
        {
            for (size_t rightIndex = 0; rightIndex < entries.size(); ++rightIndex)
            {
                if (leftIndex != rightIndex)
                {
                    const Vec2 diff = entries[rightIndex] - entries[leftIndex];
                    const Vec2 primaryAntinode = entries[leftIndex] - diff;

                    if (isInBounds(primaryAntinode))
                        priamryAntinodeLocations.emplace(primaryAntinode);

                    const Vec2 dir = diff / std::gcd(diff.x, diff.y);
                    for (Vec2 current = entries[leftIndex]; isInBounds(current); current = current + dir)
                        allAntinodeLocations.emplace(current);
                }
            }
        }
    }

    std::println("Num primary antinode locations: {}", priamryAntinodeLocations.size());
    std::println("Num all antinode locations: {}", allAntinodeLocations.size());
}
