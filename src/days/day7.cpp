#include "aoc_days.h"

#include <vector>
#include <ranges>

namespace
{
    inline int64_t concatInts(int64_t l, int64_t r)
    {
        for (int64_t n = r; n; n /= 10)
            l *= 10;
        return l + r;
    }

    using TSequence = std::vector<int64_t>;
    bool processSequence(TSequence::const_iterator seqItr, TSequence::const_iterator seqEnd, int64_t currentTotal, const int64_t target, bool allowConcat)
    {
        if (currentTotal == target)
            return true;

        if (++seqItr != seqEnd)
        {
            if (processSequence(seqItr, seqEnd, currentTotal + *seqItr, target, allowConcat))
                return true;
            else if (processSequence(seqItr, seqEnd, currentTotal * *seqItr, target, allowConcat))
                return true;
            else if (allowConcat && processSequence(seqItr, seqEnd, concatInts(currentTotal, *seqItr), target, allowConcat))
                return true;
        }
        return false;
    }
}

void aoc::day7(std::string_view inputFilePath)
{
    size_t totalCalibrationResult = 0, totalConcatCalibrationResult = 0;

    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line);)
    {
        const size_t colonPos = line.find(':');
        const auto inputs = std::ranges::to<std::vector<int64_t>>(line.substr(colonPos + 2) | std::views::split(' ') | std::views::transform([](auto&& r) -> int64_t {
            return std::stoll(std::string(r.begin(), r.end()));
            }));

        const int64_t answer = std::stoll(line.substr(0, colonPos));
        if (processSequence(inputs.cbegin(), inputs.cend(), *inputs.cbegin(), answer, false))
            totalCalibrationResult += answer;

        if (processSequence(inputs.cbegin(), inputs.cend(), *inputs.cbegin(), answer, true))
            totalConcatCalibrationResult += answer;
    }

    std::println("Total calibration result: {}", totalCalibrationResult);
    std::println("Total calibration result with concatenation: {}", totalConcatCalibrationResult);
}
