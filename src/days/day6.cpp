#include "aoc_days.h"

#include <vector>
#include <set>

namespace
{
    struct Coord
    {
        int x = 0, y = 0;
        Coord operator+(const Coord& rhs) { return Coord{ x + rhs.x, y + rhs.y }; };
    };
    bool operator<(const Coord& lhs, const Coord& rhs) { return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y); };
}

void aoc::day6(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());

    // 0 = up, 1 = right, 2 = down, 3 = left.
    Coord guardPos = { -1, -1 };
    Coord guardFacing = { 0, -1 };  // Guard always faces up to start.
    std::set<Coord> visitedPositions;

    std::vector<std::string> grid;
    for (std::string line; std::getline(file, line);)
    {
        grid.emplace_back(line);
        if (const size_t findPos = line.find('^'); findPos != std::string::npos)
        {
            guardPos = { (int)findPos, (int)grid.size() - 1 };
        }
    }

    auto isInBounds = [&grid](const Coord& c) { return c.x >= 0 && c.x < grid[0].length() && c.y >= 0 && c.y < grid.size(); };

    for (; isInBounds(guardPos);)
    {
        visitedPositions.emplace(guardPos);

        const Coord posInFront = guardPos + guardFacing;
        if (isInBounds(posInFront) && grid[posInFront.y][posInFront.x] == '#')
        {
            guardFacing = { -guardFacing.y, guardFacing.x }; // Rotate vector by 90 degrees
        }
        else // Rotate or move, not both as the guard can turn into a new obstacle
        {
            guardPos = posInFront;
        }
    }

    std::println("Number of visited positions: {}", visitedPositions.size());
}
