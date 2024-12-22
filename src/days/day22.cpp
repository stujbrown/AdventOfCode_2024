#include "aoc_days.h"

namespace
{
    int64_t solve(int64_t number)
    {
        for (int64_t i = 0; i < 2000; ++i)
        {
            int64_t operand = number * 64;
            number = number ^ operand;
            number = number % 16777216;


            operand = number / 32;
            number = number ^ operand;
            number = number % 16777216;


            operand = number * 2048;
            number = number ^ operand;
            number = number % 16777216;
        }
        return number;
    }
}

void aoc::day22(std::string_view inputFilePath)
{
    size_t secretSum = 0;
    std::ifstream file(inputFilePath.data());
    for (int64_t secretNumber = 0; file >> secretNumber;)
    {
        secretSum += (size_t)solve(secretNumber);
    }

    std::println("Secret sum: {}", secretSum);
}
