#include "aoc_days.h"
#include "vec2.h"
#include <array>
#include <vector>
#include <ranges>
#include <set>

namespace
{
    const size_t solveStartByte = 1024;
    using MemorySpace = std::array < std::array<char, 71>, 71>;

    struct State
    {
        Vec2 pos;
        size_t cost, heuristicScore;
        State* parent;

        static inline bool costSort(const State& lhs, const State& rhs) { return lhs.heuristicScore > rhs.heuristicScore; }
    };

    std::vector<Vec2> solve(const MemorySpace& memorySpace)
    {
        auto setCmp = [](const State* lhs, const State* rhs) { return lhs->pos < rhs->pos; };
        auto mdist = [](const Vec2& lhs, const Vec2& rhs) -> size_t { return (std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y)); };

        const Vec2 endPos{ (int)memorySpace[0].size() - 1, (int)memorySpace.size() - 1 };

        std::vector<std::unique_ptr<State>> stateMemory;
        std::set<State*, decltype(setCmp)> visited;
        std::vector<State> queue;
        queue.emplace_back(State{ .pos{0, 0}, .cost = 0, .heuristicScore = mdist({0, 0}, endPos), .parent = nullptr });
        while (!queue.empty())
        {
            State current = std::move(queue.back());
            queue.pop_back();

            if (current.pos.x == endPos.x && current.pos.y == endPos.y)
            {
                std::vector<Vec2> outputPath;
                for (const State* next = &current; next; next = next->parent)
                    outputPath.emplace_back(next->pos);
                return outputPath;
            }

            auto existingItr = visited.find(&current);
            if (existingItr != visited.end())
            {
                if (current.cost >= (*existingItr)->cost)
                    continue;
                throw std::logic_error{ "Should never hit a lower cost case" };
            }

            State* currentPtr = new State(current);
            stateMemory.emplace_back(std::unique_ptr<State>(currentPtr));
            visited.emplace(currentPtr);

            for (const Vec2& dir : std::initializer_list<Vec2>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} })
            {
                const Vec2 newPos = currentPtr->pos + dir;
                if (newPos.x >= 0 && newPos.x < memorySpace[0].size() && newPos.y >= 0 && newPos.y < memorySpace.size() &&
                    memorySpace[newPos.y][newPos.x] != '#')
                {
                    const State newState{ .pos = newPos, .cost = currentPtr->cost + 1, .heuristicScore = currentPtr->cost + mdist(newPos, endPos), .parent = currentPtr };
                    queue.insert(std::upper_bound(queue.begin(), queue.end(), newState, State::costSort), newState);
                }
            }
        }
        return {};
    }
}

void aoc::day18(std::string_view inputFilePath)
{
    MemorySpace memorySpace;
    for (auto& a : memorySpace)
        a.fill('.');

    std::vector<Vec2> lastBestRoute;
    int byteCount = 0;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line); ++byteCount)
    {
        Vec2 coord;
        auto splitViewItr = (line | std::views::split(',')).begin();
        std::string_view sX(*splitViewItr++), sY(*splitViewItr);
        std::from_chars(sX.data(), sX.data() + sX.length(), coord.x);
        std::from_chars(sY.data(), sY.data() + sY.length(), coord.y);

        memorySpace[coord.y][coord.x] = '#';

        if (byteCount == solveStartByte - 1)
        {
            lastBestRoute = solve(memorySpace);
            std::println("Shortest path in steps: {}", lastBestRoute.size() - 1);
        }
        else if (byteCount > solveStartByte - 1)
        {
            if (std::find(lastBestRoute.begin(), lastBestRoute.end(), coord) != lastBestRoute.end())
            {
                lastBestRoute = solve(memorySpace);
                if (lastBestRoute.size() == 0)
                {
                    std::println("Byte which will block the exit: {},{}", coord.x, coord.y);
                    return;
                }
            }
        }
    }
}
