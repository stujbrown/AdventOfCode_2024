#include "aoc_days.h"
#include "vec2.h"
#include <vector>

namespace
{
    void solve(std::vector<std::string>& map, const std::vector<char>& instructions)
    {
        Vec2 robotPos{};
        for (size_t y = 0; y < map.size(); ++y)
            for (size_t x = 0; x < map[0].length(); ++x)
                if (map[y][x] == '@')
                    robotPos = { (int)x, (int)y };

        for (char c : instructions)
        {
            Vec2 dir{};
            if (c == '<') dir = { -1, 0 };
            else if (c == '>') dir = { 1, 0 };
            else if (c == '^') dir = { 0, -1 };
            else if (c == 'v') dir = { 0, 1 };

            const Vec2 next = robotPos + dir;
            if (map[next.y][next.x] == 'O' || map[next.y][next.x] == '[' || map[next.y][next.x] == ']')
            {
                std::vector<Vec2> affectedBoxCells, pushCheckQueue;
                pushCheckQueue.push_back(next);
                bool canPush = true;
                while (!pushCheckQueue.empty())
                {
                    const auto pushNext = pushCheckQueue.front();
                    const char nextValue = map[pushNext.y][pushNext.x];
                    pushCheckQueue.erase(pushCheckQueue.begin());

                    if (std::find(affectedBoxCells.begin(), affectedBoxCells.end(), pushNext) != affectedBoxCells.end() ||
                        nextValue == '.')
                        continue;

                    if (nextValue == '#')
                    {
                        canPush = false;
                        break;
                    }

                    affectedBoxCells.emplace_back(pushNext);

                    if (nextValue == 'O' || nextValue == '[' || nextValue == ']')
                    {
                        pushCheckQueue.emplace_back(pushNext + dir);
                        if (nextValue == '[' && dir.y != 0)
                            pushCheckQueue.emplace_back(pushNext + Vec2{ 1, 0 });
                        else if (nextValue == ']' && dir.y != 0)
                            pushCheckQueue.emplace_back(pushNext + Vec2{ -1, 0 });
                    }
                }

                if (canPush)
                {
                    for (auto itr = affectedBoxCells.rbegin(); itr != affectedBoxCells.rend(); ++itr)
                    {
                        map[itr->y + dir.y][itr->x + dir.x] = map[itr->y][itr->x];
                        map[itr->y][itr->x] = '.';
                    }
                }
            }

            if (map[next.y][next.x] == '.')
            {
                map[next.y][next.x] = '@';
                map[robotPos.y][robotPos.x] = '.';
                robotPos = next;
            }
        }
    }
}


void aoc::day15(std::string_view inputFilePath)
{
    std::vector<std::string> map, bigMap;
    std::vector<char> instructions;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line) && line[0] == '#';)
    {
        map.emplace_back(line);
        bigMap.push_back({});
        for (char c : line)
        {
            if (c == 'O') bigMap.back().append("[]");
            else if (c == '#') bigMap.back().append("##");
            else if (c == '@') bigMap.back().append("@.");
            else bigMap.back().append("..");
        }
    }

    for (std::string line; std::getline(file, line);)
        for (char c : line)
            instructions.emplace_back(c);

    solve(map, instructions);
    solve(bigMap, instructions);

    auto sum = [](const std::vector<std::string >& map) {
        size_t boxGpsSum = 0;
        for (size_t y = 0; y < map.size(); ++y)
            for (size_t x = 0; x < map[0].length(); ++x)
                if (map[y][x] == 'O' || map[y][x] == '[') boxGpsSum += 100 * y + x;
        return boxGpsSum;
        };

    std::println("Box GPS sum: {}", sum(map));
    std::println("Box GPS sum for larger warehouse: {}", sum(bigMap));
}
