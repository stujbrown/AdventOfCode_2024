#include "aoc_days.h"
#include "vec2.h"
#include <vector>
#include <regex>
#include <array>
#include <set>


namespace
{
    const int width = 101, height = 103;
    struct Robot
    {
        Vec2 position;
        Vec2 velocity;
    };

    void step(std::vector<Robot>& robots)
    {
        for (Robot& robot : robots)
        {
            robot.position += robot.velocity;
            if (robot.position.x >= width || robot.position.x < 0) 
                robot.position.x = (robot.position.x + width) % width;
            if (robot.position.y >= height || robot.position.y < 0) 
                robot.position.y = (robot.position.y + height) % height;
        }
    }
}

void aoc::day14(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());

    std::vector<Robot> robots;
    for (std::string line; std::getline(file, line);)
    {
        std::smatch match;
        std::regex_search(line, match, std::regex("p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)"));
        robots.emplace_back(Robot{ .position = {std::stoi(match[1]), std::stoi(match[2])}, .velocity = {std::stoi(match[3]), std::stoi(match[4])} });
    }

    std::vector<Robot> robotsCopy = robots;
    for (int second = 0; second < 100; ++second)
        step(robotsCopy);

    const int midColumn = ((width - 1) / 2), midRow = ((height - 1) / 2);
    std::array<size_t, 4> quadrantCounts{ 0,0,0,0 };
    for (Robot& robot : robots)
    {
        if (robot.position.x < midColumn && robot.position.y < midRow) ++quadrantCounts[0];
        else if (robot.position.x > midColumn && robot.position.y < midRow) ++quadrantCounts[1];
        else if (robot.position.x < midColumn && robot.position.y > midRow) ++quadrantCounts[2];
        else if (robot.position.x > midColumn && robot.position.y > midRow) ++quadrantCounts[3];
    }

    for (int secondsPassed = 1; true; ++secondsPassed)
    {
        step(robots);

        std::set<Vec2> positions;
        for (Robot& robot : robots)
            positions.emplace(robot.position);

        if (positions.size() == robots.size())
        {
            std::println("Seconds for tree to form: {}", secondsPassed);
            break;
        }
    }

    std::println("Safety factor: {}", quadrantCounts[0] * quadrantCounts[1] * quadrantCounts[2] * quadrantCounts[3]);
}
