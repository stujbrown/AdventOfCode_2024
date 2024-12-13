#include "aoc_days.h"
#include <regex>

size_t solve(int64_t aX, int64_t aY, int64_t bX, int64_t bY, int64_t prizeX, int64_t prizeY)
{
    // [na + mb = p] where p = prize, n/m = presses on either a or b, where a b and p are 2D vectors
    const double n = (((prizeX / (double)bX) * bY) - prizeY) / (((aX / (double)bX) * bY) - aY); 
    const double m = (prizeY - (aY * n)) / (double)bY;

    const size_t pressesA = (size_t)std::round(n);
    const size_t pressesB = (size_t)std::round(m);
    return ((std::fabs(n - pressesA) < 0.001f) && std::fabs(m - pressesB) < 0.001f) ?
        (pressesA * 3 + pressesB) : 0;
}

void aoc::day13(std::string_view inputFilePath)
{
    size_t minTokenSpend = 0, farPrizeMinTokenSpend = 0;

    std::ifstream file(inputFilePath.data());
    for (std::string l1, l2, l3; std::getline(file, l1) && std::getline(file, l2) && std::getline(file, l3); std::getline(file, l1))
    {
        auto parseInput = [](const std::string& input, const std::string& regex) -> std::pair<int64_t, int64_t> {
            std::smatch match;
            std::regex_search(input, match, std::regex(regex));
            return std::make_pair(std::stoi(match[1]), std::stoi(match[2])); 
        };

        const auto buttonA = parseInput(l1, "X\\+(\\d+), Y\\+(\\d+)");
        const auto buttonB = parseInput(l2, "X\\+(\\d+), Y\\+(\\d+)");
        const auto prize = parseInput(l3, "X=(\\d+), Y=(\\d+)");

        minTokenSpend += solve(buttonA.first, buttonA.second, buttonB.first, buttonB.second, prize.first, prize.second);
        farPrizeMinTokenSpend += solve(buttonA.first, buttonA.second, buttonB.first, buttonB.second, prize.first + 10000000000000, prize.second + 10000000000000);
    }

    std::println("Min token spend to win prizes: {}", minTokenSpend);
    std::println("Min token spend to win prizes at higher distances: {}", farPrizeMinTokenSpend);
}
