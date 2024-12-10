#include "aoc_days.h"
#include "vec2.h"

#include <vector>
#include <set>

struct HoofItSolver
{
    size_t score = 0, rating = 0;

    void solve(const std::vector<std::string>& map, std::set<Vec2>& reachedTrailheads, Vec2 pos)
    {
        if (map[pos.y][pos.x] == '9')
        {
            score += reachedTrailheads.insert(pos).second ? 1 : 0;
            rating += 1;
            return;
        }

        for (const Vec2& direction : std::initializer_list<Vec2>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} })
        {
            const Vec2 newPos = { pos.x + direction.x, pos.y + direction.y };
            if ((newPos.x >= 0 && newPos.x < map[0].length() && newPos.y >= 0 && newPos.y < map.size()) && map[newPos.y][newPos.x] == map[pos.y][pos.x] + 1)
                solve(map, reachedTrailheads, Vec2{ newPos.x, newPos.y });
        }
    }
};

void aoc::day10(std::string_view inputFilePath)
{
    std::vector<std::string> map;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line);)
        map.emplace_back(line);

    HoofItSolver solver;
    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map[0].length(); ++x)
        {
            if (map[y][x] == '0')
            {
                std::set<Vec2> reachedTrailheads;
                solver.solve(map, reachedTrailheads, Vec2{ x, y });
            }
        }
    }
    std::println("Trailhead score: {}", solver.score);
    std::println("Trailhead rating: {}", solver.rating);
}
