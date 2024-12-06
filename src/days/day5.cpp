#include "aoc_days.h"

#include <vector>
#include <map>
#include <ranges>

void aoc::day5(std::string_view inputFilePath)
{
    using IntPair = std::pair<int, int>;

    std::ifstream file(inputFilePath.data());
    std::multimap<int, int> pageMustFollowLookup;

    for (std::string line; std::getline(file, line) && line.find_first_not_of(' ') != std::string::npos;)
    {
        const std::vector<std::string> pageRule = std::ranges::to<std::vector<std::string>>(line | std::views::split('|'));
        pageMustFollowLookup.insert(std::make_pair(std::stoi(pageRule[1]), std::stoi(pageRule[0])));
    }

    int middlePageTotal = 0, reorderedMiddlePageTotal = 0;
    for (std::string line; std::getline(file, line);)
    {
        const std::vector<int> pageSequence = std::ranges::to<std::vector<int>>((line | std::views::split(',')) |
            std::ranges::views::transform([](const auto& s) { return std::stoi(std::ranges::to<std::string>(s)); })
        );

        auto orderedInsertComp = [](const IntPair& lhs, const IntPair& rhs) { return lhs.first < rhs.first; };
        std::vector<IntPair> reordered;

        bool outOfOrder = false;
        for (size_t i = 0; i < pageSequence.size(); ++i)
        {
            const int currentPage = pageSequence[i];
            auto mustComeAfterItr = pageMustFollowLookup.equal_range(currentPage);

            int priority = 0;
            for (size_t innerIndex = 0; innerIndex < pageSequence.size(); ++innerIndex)
            {
                if (std::find_if(mustComeAfterItr.first, mustComeAfterItr.second, [&](const IntPair& x) {
                    return x.second == pageSequence[innerIndex]; }) != mustComeAfterItr.second)
                {
                    ++priority;
                    outOfOrder |= innerIndex > i;
                }
            }

            const IntPair newEntry = std::make_pair(priority, currentPage);
            reordered.insert(std::lower_bound(reordered.begin(), reordered.end(), newEntry, orderedInsertComp), newEntry);
        }

        if (!outOfOrder)
            middlePageTotal += pageSequence[pageSequence.size() / 2];
        else
            reorderedMiddlePageTotal += reordered[reordered.size() / 2].second;

        std::println("Sequence {} is {}", line, outOfOrder ? "invalid" : "valid");
    }

    std::println("Middle page total: {}", middlePageTotal);
    std::println("Middle page total for reordered: {}", reorderedMiddlePageTotal);
}
