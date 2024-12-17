#include "aoc_days.h"
#include <ranges>
#include <vector>

namespace
{
    inline int64_t getComboOperand(int v, const std::array<int64_t, 3>& registers)
    {
        if (v == 4) return registers[0];
        else if (v == 5) return registers[1];
        else if (v == 6) return registers[2];
        return v;
    }

    void run(const std::vector<int>& instructions, std::array<int64_t, 3> registers, std::vector<int>* output)
    {
        output->clear();
        for (size_t i = 0; i < instructions.size(); i += 2)
        {
            if (instructions[i] == 0) // adv
            {
                registers[0] = registers[0] / (int64_t)std::pow(2, getComboOperand(instructions[i + 1], registers));
            }
            else if (instructions[i] == 1) // bxl
            {
                registers[1] = registers[1] ^ instructions[i + 1];
            }
            else if (instructions[i] == 2) // bst
            {
                registers[1] = getComboOperand(instructions[i + 1], registers) % 8;
            }
            else if (instructions[i] == 3) // jnz
            {
                if (registers[0] != 0)
                    i = instructions[i + 1] - 2;
            }
            else if (instructions[i] == 4) // bxc
            {
                registers[1] = registers[1] ^ registers[2];
            }
            else if (instructions[i] == 5) // out
            {
                output->push_back(getComboOperand(instructions[i + 1], registers) % 8);
            }
            else if (instructions[i] == 6) // bdv
            {
                registers[1] = registers[0] / (int64_t)std::pow(2, getComboOperand(instructions[i + 1], registers));
            }
            else if (instructions[i] == 7) // cdv
            {
                registers[2] = registers[0] / (int64_t)std::pow(2, getComboOperand(instructions[i + 1], registers));
            }
        }
    }
}

void aoc::day17(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());

    std::array<int64_t, 3> registers{};
    std::string s;
    for (int i = 0, v = 0; i < 3 && file >> s && file >> s && file >> v; ++i)
        registers[i] = v;

    file >> s && file >> s && file >> s;
    const auto instructions = std::ranges::to<std::vector<int>>(std::views::split(std::string_view(s), ',') | std::views::transform([](auto&& x) { return std::stoi(std::string(x.begin(), x.end())); }));

    std::vector<int> originalOutput;
    run(instructions, registers, &originalOutput);


    for (int i = 0; i < originalOutput.size(); ++i)
        std::cout << originalOutput[i] << ((i != originalOutput.size() - 1) ? "," : "");
    std::cout << std::endl;
}
