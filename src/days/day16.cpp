#include "aoc_days.h"
#include "vec2.h"
#include <vector>
#include <list>
#include <set>

namespace
{
    struct State
    {
        Vec2 pos;
        Vec2 dir;
        size_t cost;

        std::vector<std::shared_ptr<State>> previous;
    };

    struct StateSetCompare
    {
        constexpr inline bool operator()(const std::shared_ptr<State>& lhs, const std::shared_ptr<State>& rhs) const
        {
            return std::tie(lhs->pos, lhs->dir) < std::tie(rhs->pos, rhs->dir); // Ignore cost and path
        }
    };

    inline bool costSorter(const std::shared_ptr<State>& lhs, const std::shared_ptr<State>& rhs)
    {
        return lhs->cost > rhs->cost;
    }
}

void aoc::day16(std::string_view inputFilePath)
{
    Vec2 startPos{}, endPos{};
    std::vector<std::string> map;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line);)
    {
        map.emplace_back(line);
        if (size_t startIndex = line.find('S'); startIndex != std::string::npos)
            startPos = { (int)startIndex, (int)map.size() - 1 };

        if (size_t endIndex = line.find('E'); endIndex != std::string::npos)
            endPos = { (int)endIndex, (int)map.size() - 1 };
    }

    size_t lowestCost = std::numeric_limits<size_t>::max();
    std::vector<std::shared_ptr<State>> queue, finalStates;
    std::set<std::shared_ptr<State>, StateSetCompare> visited;
    queue.emplace_back(new State{ .pos = startPos, .dir = Vec2{ 1, 0 }, .cost = 0 });
    while (!queue.empty())
    {
        std::shared_ptr<State> current = queue.back();
        queue.pop_back();

        if (current->pos == endPos)
        {
            if (lowestCost == std::numeric_limits<size_t>::max())
                lowestCost = current->cost;

            if (current->cost == lowestCost)
                finalStates.emplace_back(current);
            continue;
        }

        bool didMerge = false;
        auto existing = visited.find(current);
        if (existing != visited.end())
        {
            if (current->cost > (*existing)->cost)
                continue;
            else if (current->cost == (*existing)->cost)
            {
                // Merge nodes together
                auto oldNode = *existing;
                for (auto otherPrevious : current->previous)
                    oldNode->previous.emplace_back(otherPrevious);

                current = oldNode;
                didMerge = true;
            }
            else
                visited.erase(current);
        }
        else if (!didMerge)
            visited.emplace(current);

        for (const Vec2 dir : std::initializer_list{ current->dir, { -current->dir.y, current->dir.x }, { current->dir.y, -current->dir.x } })
        {
            const Vec2 newPos = current->pos + dir;
            if (map[newPos.y][newPos.x] != '#')
            {
                std::shared_ptr<State> newState(new State{ .pos = newPos, .dir = dir, .cost = current->cost + ((current->dir == dir) ? 1u : 1001u) });
                newState->previous.push_back(current);
                queue.insert(std::upper_bound(queue.cbegin(), queue.cend(), newState, costSorter), newState);
            }
        }
    }

    // Walk routes back
    std::set<Vec2> bestRouteTiles;
    visited.clear();
    queue.clear();
    for (auto state : finalStates)
        queue.push_back(state);
    while (!queue.empty())
    {
        std::shared_ptr<State> current = queue.back();
        queue.pop_back();

        if (visited.contains(current))
            continue;
        visited.emplace(current);

        bestRouteTiles.emplace(current->pos);
        for (auto p : current->previous)
            queue.push_back(p);
    }

    std::println("Lowest cost route: {}", lowestCost);
    std::println("Num tiles on best routes: {}", bestRouteTiles.size());
}
