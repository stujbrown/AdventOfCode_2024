#include "aoc_days.h"

#include <regex>

void aoc::day3(std::string_view inputFilePath)
{
    std::ifstream file(inputFilePath.data());
    const std::string fileContents{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

    const std::regex pattern("(do\\(\\)|don't\\(\\)|mul\\((\\d+),(\\d+)\\))");
    int total = 0, totalWithDoAndDontRules = 0;
    bool mulsEnabled = true;

    const char* stringPos = fileContents.c_str();
    for (std::cmatch match; std::regex_search(stringPos, match, pattern);)
    {
        const char* matchedStr = match[0].first;
        if (matchedStr[0] == 'm') // mul
        {
            int operands[2] = { 0, 0 };
            std::from_chars(match[2].first, match[2].second, operands[0]);
            std::from_chars(match[3].first, match[3].second, operands[1]);
            const int result = operands[0] * operands[1];

            total += result;
            totalWithDoAndDontRules += mulsEnabled ? result : 0;
        }
        else
            mulsEnabled = matchedStr[2] == 'n' ? false : true; // don't/do

        stringPos = match.suffix().first;
    }

    std::println("Total: {}", total);
    std::println("Total with do and don't rules: {}", totalWithDoAndDontRules);    
}
