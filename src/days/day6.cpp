#include "aoc_days.h"

#include <vector>
#include <set>
#include <optional>

namespace
{
    struct Coord
    {
        int x = 0, y = 0;
        Coord operator+(const Coord& rhs) { return Coord{ x + rhs.x, y + rhs.y }; };
    };
    bool operator<(const Coord& lhs, const Coord& rhs) { return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y); };
    bool operator==(const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; };

    bool isInBounds(const std::vector<std::string>& grid, const Coord& c) { return c.x >= 0 && c.x < grid[0].length() && c.y >= 0 && c.y < grid.size(); };

    std::optional<std::pair<size_t, size_t>> simulate(const std::vector<std::string>& grid, const Coord& guardStartPos, Coord guardFacing, const std::optional<Coord>& blocker = std::nullopt)
    {
        Coord guardPos = guardStartPos;

        std::set<Coord> visitedPositions;
        std::set<Coord> successfulBlockerPositions;
        std::set<std::pair<Coord, Coord>> visitedPositionsWithDirections;
        for (; isInBounds(grid, guardPos);)
        {
            visitedPositions.emplace(guardPos);

            const Coord posInFront = guardPos + guardFacing;
            if (isInBounds(grid, posInFront) && (grid[posInFront.y][posInFront.x] == '#' || (blocker.has_value() && posInFront == blocker)))
            {
                guardFacing = { -guardFacing.y, guardFacing.x }; // Rotate vector by 90 degrees
            }
            else // Rotate or move, not both as the guard can turn into a new obstacle
            {
                if (!blocker.has_value() && isInBounds(grid, posInFront) && !successfulBlockerPositions.contains(posInFront) && posInFront != guardStartPos)
                {
                    // Start a sub-simulation if not already in one
                    if (!simulate(grid, guardStartPos, Coord{ 0, -1 }, posInFront).has_value()) // Place blocker immediately in-front of the guard to cause a deviation
                    {
                        successfulBlockerPositions.emplace(posInFront);
                    }
                }

                guardPos = posInFront;

                const auto guardPosAndFacing = std::make_pair(guardPos, guardFacing);
                if (visitedPositionsWithDirections.contains(guardPosAndFacing))
                    return std::nullopt;

                visitedPositionsWithDirections.emplace(guardPosAndFacing);
            }
        }

        return std::make_pair(visitedPositions.size(), successfulBlockerPositions.size());
    }
}

void aoc::day6(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());

    // 0 = up, 1 = right, 2 = down, 3 = left.
    Coord guardPos = { -1, -1 };
    Coord guardFacing = { 0, -1 };  // Guard always faces up to start.

    std::vector<std::string> grid;
    for (std::string line; std::getline(file, line);)
    {
        grid.emplace_back(line);
        if (const size_t findPos = line.find('^'); findPos != std::string::npos)
        {
            guardPos = { (int)findPos, (int)grid.size() - 1 };
        }
    }

    const std::optional<std::pair<size_t, size_t>> visitedPositions = simulate(grid, guardPos, guardFacing);

    std::println("Number of visited positions: {}", visitedPositions->first);
    std::println("Number of possible blocker positions: {}", visitedPositions->second);
}
