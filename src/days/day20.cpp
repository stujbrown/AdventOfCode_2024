#include "aoc_days.h"
#include "vec2.h"
#include <vector>
#include <set>
#include <map>

namespace
{
    struct State
    {
        Vec2 pos;
        size_t cost = 0;
        State* parent = nullptr;

        static inline bool costSort(const State& lhs, const State& rhs) { return lhs.cost > rhs.cost; }
    };

    std::vector<State> solve(const std::vector<std::string>& map, const Vec2& startPos, const Vec2& endPos)
    {
        auto statePosCmpPtr = [](const State* lhs, const State* rhs) {
            return lhs->pos < rhs->pos;
            };

        std::vector<std::unique_ptr<State>> allStates;
        std::set<State*, decltype(statePosCmpPtr)> visited;
        std::vector<State> queue;
        queue.emplace_back(State{ .pos = startPos });
        while (!queue.empty())
        {
            State current = std::move(queue.back());
            queue.pop_back();

            if (current.pos == endPos)
            {
                std::vector<State> outputRoute;
                for (const State* next = &current; next; next = next->parent)
                    outputRoute.emplace_back(State{ .pos = next->pos, .cost = next->cost });
                return outputRoute;
            }

            auto existingItr = visited.find(&current);
            if (existingItr != visited.end())
            {
                if (current.cost >= (*existingItr)->cost)
                    continue;
                throw std::logic_error{ "Should never hit a lower cost case" };
            }

            State* currentPtr = new State(current);
            allStates.emplace_back(std::unique_ptr<State>(currentPtr));
            visited.emplace(currentPtr);

            for (const Vec2& dir : std::initializer_list<Vec2>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} })
            {
                const Vec2 newPos = currentPtr->pos + dir;
                if (newPos.x >= 0 && newPos.x < map[0].size() && newPos.y >= 0 && newPos.y < map.size())
                {
                    if (map[newPos.y][newPos.x] != '#')
                    {
                        const State newState{ .pos = newPos, .cost = currentPtr->cost + 1, .parent = currentPtr };
                        queue.insert(std::upper_bound(queue.begin(), queue.end(), newState, State::costSort), newState);
                    }
                }
            }
        }
        return {};
    }

    size_t countPossibleCheats(const std::vector<State>& path, int maxCheatLength)
    {
        std::map<std::pair<Vec2, Vec2>, size_t> successfulCheats;
        for (size_t i = 0; i < path.size(); ++i)
        {
            for (size_t j = 0; j < path.size(); ++j)
            {
                if (i != j)
                {
                    const size_t cheatDistance = std::abs(path[i].pos.x - path[j].pos.x) + std::abs(path[i].pos.y - path[j].pos.y);
                    if (cheatDistance > maxCheatLength)
                        continue;

                    if (path[i].cost < path[j].cost)
                    {
                        const size_t skippedDistance = path[j].cost - path[i].cost;
                        const int64_t timeSaved = ((int64_t)skippedDistance - cheatDistance);

                        if (timeSaved >= 100)
                        {
                            const auto cheat = std::make_pair(path[i].pos, path[j].pos);
                            if (auto itr = successfulCheats.find(cheat); itr != successfulCheats.end())
                            {
                                if ((size_t)timeSaved > itr->second)
                                    successfulCheats[cheat] = timeSaved;
                            }
                            else
                                successfulCheats[cheat] = timeSaved;
                        }
                    }
                }
            }

        }
        return successfulCheats.size();
    }
}

void aoc::day20(std::string_view inputFilePath)
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

    std::vector<State> legitimatePath = solve(map, startPos, endPos);
    const size_t cheatsThatMeetThreshold = countPossibleCheats(legitimatePath, 2);
    const size_t longCheatsThatMeetThreshold = countPossibleCheats(legitimatePath, 20);

    std::println("Basic cheats that save 100 picoseconds: {}", cheatsThatMeetThreshold);
    std::println("Long cheats that save 100 picoseconds: {}", longCheatsThatMeetThreshold);
}
