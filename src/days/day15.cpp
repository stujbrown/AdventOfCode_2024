#include "aoc_days.h"
#include "vec2.h"
#include <vector>

void aoc::day15(std::string_view inputFilePath)
{
    std::vector<std::string> map;
    Vec2 robotPos{};
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line) && line[0] == '#';)
    {
        map.emplace_back(line);
        if (size_t pos = line.find('@'); pos != std::string::npos)
        {
            robotPos = { (int)pos, (int)map.size() - 1 };
        }
    }

    for (std::string line; std::getline(file, line);)
    {
        for (char c : line)
        {
            Vec2 dir{};
            if (c == '<') dir = { -1, 0 };
            else if (c == '>') dir = { 1, 0 };
            else if (c == '^') dir = { 0, -1 };
            else if (c == 'v') dir = { 0, 1 };

            const Vec2 next = robotPos + dir;
            if (map[next.y][next.x] == 'O')
            {
                Vec2 pushNext;
                for (pushNext = next + dir; map[pushNext.y][pushNext.x] == 'O'; pushNext += dir);
                if (map[pushNext.y][pushNext.x] != '#')
                {
                    map[pushNext.y][pushNext.x] = 'O';
                    map[next.y][next.x] = '.';
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

    size_t boxGpsSum = 0;
    for (size_t y = 0; y < map.size(); ++y)
        for (size_t x = 0; x < map[0].length(); ++x)
            if (map[y][x] == 'O') boxGpsSum += 100 * y + x;

    std::println("Box GPS sum: {}", boxGpsSum);
}
