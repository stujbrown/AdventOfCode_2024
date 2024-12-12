#include "aoc_days.h"
#include "vec2.h"

#include <vector>
#include <set>

void aoc::day12(std::string_view inputFilePath)
{
    std::vector<std::string> garden;
    std::ifstream file(inputFilePath.data());
    for (std::string row; std::getline(file, row);)
        garden.emplace_back(row);

    auto isInBounds = [](const std::vector<std::string>& garden, const Vec2& v) { return v.x >= 0 && v.x < garden[0].length() && v.y >= 0 && v.y < garden.size(); };

    size_t totalCost = 0, totalBulkCost = 0;
    std::set<Vec2> plantsEvaluated;

    for (int y = 0; y < garden.size(); ++y)
        for (int x = 0; x < garden[0].length(); ++x)
        {
            const char plant = garden[y][x];
            if (!plantsEvaluated.contains(Vec2{ x, y }))
            {
                size_t numPlants = 0, numFences = 0, numEdges = 0;
                std::vector<Vec2> queue(1, Vec2{ x, y });
                while (!queue.empty())
                {
                    const Vec2 current = queue.back();
                    queue.pop_back();
                    if (plantsEvaluated.contains(current))
                        continue;

                    ++numPlants;
                    plantsEvaluated.insert(current);
                    for (const Vec2 neighbourOffset : std::initializer_list{ Vec2{ 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } })
                    {
                        if (const Vec2 neighbour = (current + neighbourOffset); isInBounds(garden, neighbour) && garden[neighbour.y][neighbour.x] == plant)
                            queue.emplace_back(neighbour);
                        else
                            ++numFences;
                    }

                    for (const Vec2 diagonalNeighbourOffset : std::initializer_list{ Vec2{ 1, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 } })
                    {
                        const Vec2 a1{ current.x, current.y + diagonalNeighbourOffset.y };
                        const Vec2 a2{ current.x + diagonalNeighbourOffset.x, current.y };
                        size_t adjacentCount = (isInBounds(garden, a1) && garden[a1.y][a1.x] == plant);
                        adjacentCount += (isInBounds(garden, a2) && garden[a2.y][a2.x] == plant);

                        if (const Vec2 diagonalNeighbour = (current + diagonalNeighbourOffset);
                            (adjacentCount == 0) || (adjacentCount == 2 && garden[diagonalNeighbour.y][diagonalNeighbour.x] != plant))
                            ++numEdges;
                    }
                }
                totalCost += numPlants * numFences;
                totalBulkCost += numPlants * numEdges;
            }
        }
    std::println("Total cost: {}", totalCost);
    std::println("Total cost with bulk discount: {}", totalBulkCost);

}
