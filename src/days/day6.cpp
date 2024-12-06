#include "aoc_days.h"

#include <vector>
#include <set>
#include <optional>
#include <algorithm>
#include <execution>

namespace
{
    struct Coord { int x = 0, y = 0; };
    bool operator<(const Coord& lhs, const Coord& rhs) { return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y); };
    bool operator==(const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; };
    inline Coord operator+(const Coord& lhs, const Coord& rhs) { return Coord{ lhs.x + rhs.x, lhs.y + rhs.y }; };

    inline bool isInBounds(const Coord& gridSize, const Coord& c) { return c.x >= 0 && c.x < gridSize.x && c.y >= 0 && c.y < gridSize.y; };
    inline int index2DTo1D(const Coord& c, int gridWidth) { return c.y * gridWidth + c.x; };
}

void aoc::day6(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());

    Coord guardStartPos = { -1, -1 };
    Coord guardStartFacing = { 0, -1 };

    std::vector<char> grid;
    std::string line;
    for (int lineIndex = 0; std::getline(file, line); ++lineIndex)
    {
        grid.append_range(line);
        if (const size_t findPos = line.find('^'); findPos != std::string::npos)
            guardStartPos = { (int)findPos, lineIndex };
    }
    const Coord gridSize = { (int)line.length(), (int)grid.size() / (int)line.length() };


    std::vector<int> timesPositionsVisited; // 4 visits means another valid direction can't be taken without looping
    timesPositionsVisited.resize(grid.size());

    auto simulate = [&timesPositionsVisited](std::set<Coord>* outProspectiveBlockerPositions, const std::vector<char>& grid, const Coord& gridSize, Coord guardPos, Coord guardFacing, const std::optional<Coord>& blocker = std::nullopt) -> int
    {
        std::fill(timesPositionsVisited.begin(), timesPositionsVisited.end(), 0);

        int numVisitedPositions = 0;
        while (isInBounds(gridSize, guardPos))
        {
            const size_t timesVisitedIndex = index2DTo1D(guardPos, gridSize.x);
            const int timesVisited = timesPositionsVisited[timesVisitedIndex];
            timesPositionsVisited[timesVisitedIndex] += 1;
            if (timesVisited == 0)
                ++numVisitedPositions;
            else if (timesVisited >= 4)
                return -1;

            const Coord posInFront = guardPos + guardFacing;
            if (isInBounds(gridSize, posInFront) && (grid[index2DTo1D(posInFront, gridSize.x)] == '#' || (blocker != std::nullopt && posInFront == blocker)))
            {
                guardFacing = { -guardFacing.y, guardFacing.x }; // Rotate by 90 degrees
            }
            else
            {
                if (outProspectiveBlockerPositions != nullptr && !outProspectiveBlockerPositions->contains(posInFront) && isInBounds(gridSize, posInFront))
                    outProspectiveBlockerPositions->emplace(posInFront);

                guardPos = posInFront;
            }
        }

        return numVisitedPositions;
    };

    std::set<Coord> prospectiveBlockerPositions;
    const int numVisitedPositions = simulate(&prospectiveBlockerPositions, grid, gridSize, guardStartPos, guardStartFacing);

    std::atomic<size_t> numPossibleBlockerPositions = 0;
    std::for_each(
        std::execution::par_unseq,
        prospectiveBlockerPositions.begin(),
        prospectiveBlockerPositions.end(),
        [&simulate, &numPossibleBlockerPositions, &grid, &gridSize, &guardStartPos, &guardStartFacing](const Coord& blockerPos)
    {
        if ((blockerPos != guardStartPos) && simulate(nullptr, grid, gridSize, guardStartPos, guardStartFacing, blockerPos) == -1)
            ++numPossibleBlockerPositions;
    });

    std::println("Number of visited positions: {}", numVisitedPositions);
    std::println("Number of possible blocker positions: {}", numPossibleBlockerPositions.load());
}
