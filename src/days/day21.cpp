#include "aoc_days.h"
#include <vector>
#include <map>
#include <array>
#include <ranges>

namespace
{
    using SequenceLookup = std::map<std::pair<char, char>, std::vector<std::string>>;
    using SizeCache = std::map<std::tuple<char, char, size_t>, size_t>;

    template <size_t Size>
    SequenceLookup makeLookup(const std::array<char, Size>& pad, bool isNumpad)
    {
        SequenceLookup lookup;
        for (int fromIdx = 0; fromIdx < pad.size(); ++fromIdx)
            for (int toIdx = 0; toIdx < pad.size(); ++toIdx)
                if (pad[fromIdx] != '\0' && pad[toIdx] != '\0')
                {
                    std::vector<std::string> moves;
                    if (fromIdx != toIdx)
                    {
                        const int moveY = (toIdx / 3) - (fromIdx / 3);
                        const int moveX = (toIdx % 3) - (fromIdx % 3);

                        bool useVerticalFirst = true, useHorizontalFirst = true;
                        if (isNumpad)
                        {
                            useVerticalFirst = (!((toIdx == 10 || toIdx == 11) && ((fromIdx == 0 || fromIdx == 3 || fromIdx == 6))));
                            useHorizontalFirst = (!((toIdx == 0 || toIdx == 3 || toIdx == 6) && ((fromIdx == 10 || fromIdx == 11))));
                        }
                        else
                        {
                            useHorizontalFirst = (!(toIdx == 3 && ((fromIdx == 1 || fromIdx == 2))));
                            useVerticalFirst = (!((toIdx == 1 || toIdx == 2) && (fromIdx == 3)));
                        }

                        if (useVerticalFirst)
                        {
                            moves.push_back({});
                            if (moveY != 0) moves.back().append_range(moveY > 0 ? std::views::repeat('v', moveY) : std::views::repeat('^', -moveY));
                            if (moveX != 0) moves.back().append_range(moveX > 0 ? std::views::repeat('>', moveX) : std::views::repeat('<', -moveX));
                            moves.back().push_back('A');
                        }
                        if (useHorizontalFirst)
                        {
                            moves.push_back({});
                            if (moveX != 0) moves.back().append_range(moveX > 0 ? std::views::repeat('>', moveX) : std::views::repeat('<', -moveX));
                            if (moveY != 0) moves.back().append_range(moveY > 0 ? std::views::repeat('v', moveY) : std::views::repeat('^', -moveY));
                            moves.back().push_back('A');
                        }
                    }
                    else
                        moves.emplace_back(std::string("A"));
                    lookup.emplace(std::make_pair(pad[fromIdx], pad[toIdx]), moves);
                }

        return lookup;
    }


    size_t buildSequence(char current, char input, size_t depth, size_t maxDepth, const std::array<const SequenceLookup*, 2>& lookups, SizeCache& cache);
    size_t buildSequence(const std::string& inputSequence, size_t depth, size_t maxDepth, const std::array<const SequenceLookup*, 2>& lookups, SizeCache& cache)
    {
        size_t sequenceLength = 0;
        char current = 'A';
        for (char c : inputSequence)
        {
            sequenceLength += buildSequence(current, c, depth, maxDepth, lookups, cache);
            current = c;
        }
        return sequenceLength;
    }

    size_t buildSequence(char current, char input, size_t depth, size_t maxDepth, const std::array<const SequenceLookup*, 2>& lookups, SizeCache& cache)
    {
        const auto cacheKey = std::make_tuple(current, input, depth);
        const auto sizeItr = cache.find(cacheKey);
        if (sizeItr != cache.end())
            return sizeItr->second;

        const auto moveItr = lookups[depth == 0 ? 0 : 1]->find(std::make_pair(current, input));
        size_t lowestLength = std::numeric_limits<size_t>::max();
        for (const std::string& sequenceOption : moveItr->second)
        {
            size_t length = 0;
            if (depth != maxDepth - 1)
                length = buildSequence(sequenceOption, depth + 1, maxDepth, lookups, cache);
            else
                length = sequenceOption.size();

            if (length < lowestLength)
                lowestLength = length;
        }

        cache.emplace(cacheKey, lowestLength);
        return lowestLength;
    }
}

void aoc::day21(std::string_view inputFilePath)
{
    const SequenceLookup keypadTransitionLookup = makeLookup(std::array<char, 12>{{ '7', '8', '9', '4', '5', '6', '1', '2', '3', '\0', '0', 'A' }}, true);
    const SequenceLookup dpadTransitionLookup = makeLookup(std::array<char, 6>{{ '\0', '^', 'A', '<', 'v', '>' }}, false);
    std::array<const SequenceLookup*, 2> lookups{ { &keypadTransitionLookup, &dpadTransitionLookup } };

    size_t complexitySum = 0, complexitySum2 = 0;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line);)
    {
        SizeCache cache, cache2;
        size_t length = buildSequence(line, 0, 3, lookups, cache);
        size_t length2 = buildSequence(line, 0, 26, lookups, cache2);

        int num = 0;
        std::from_chars(line.data(), line.data() + line.length() - 1, num);
        complexitySum += num * length;
        complexitySum2 += num * length2;
    }

    std::println("Complexity sum: {}", complexitySum);
    std::println("Complexity sum for second room: {}", complexitySum2);
}
