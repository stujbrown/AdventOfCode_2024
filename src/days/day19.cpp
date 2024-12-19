#include "aoc_days.h"
#include <vector>
#include <ranges>
#include <array>
#include <optional>

namespace
{
    struct TrieNode
    {
        bool isEndOfWord = false; // True if a valid word can end here
        std::array<std::unique_ptr<TrieNode>, 26> children;
    };

    size_t match(const TrieNode& trie, std::string_view design, std::vector<std::optional<size_t>>& numMatchesForCharsRemaining)
    {
        const size_t numRemaining = design.length() - 1;
        if (numMatchesForCharsRemaining[numRemaining].has_value())
            return *numMatchesForCharsRemaining[numRemaining];

        size_t count = 0;
        const TrieNode* current = &trie;
        for (size_t i = 0; i < design.length(); ++i)
        {
            current = current->children[design[i] - 'a'].get();
            if (current == nullptr)
            {
                numMatchesForCharsRemaining[numRemaining] = count;
                return count;
            }

            if (current->isEndOfWord)
            {
                if (i + 1 < design.length())
                    count += match(trie, std::string_view(design.data() + (i + 1), design.length() - (i + 1)), numMatchesForCharsRemaining);
                else
                    ++count;
            }
        }

        numMatchesForCharsRemaining[numRemaining] = numMatchesForCharsRemaining[numRemaining].value_or(0) + count;
        return count;
    }
}

void aoc::day19(std::string_view inputFilePath)
{
    std::vector<std::string> availableTowels, designsToMatch;
    TrieNode trie;

    std::ifstream file(inputFilePath.data());
    std::string line;
    std::getline(file, line);
    for (auto split : line | std::views::split(',') | std::views::transform([](auto&& s) {
        return *s.begin() == ' ' ? std::string_view(s.begin() + 1, s.end()) : std::string_view(s);
    }))
    {
        TrieNode* current = &trie;
        for (const char c : std::string_view(split))
        {
            if (current->children[c - 'a'] == nullptr)
                current->children[c - 'a'] = std::make_unique<TrieNode>();
            current = current->children[c - 'a'].get();
        }
        current->isEndOfWord = true;
    }

    size_t numValid = 0, totalPermutations = 0;
    while (file >> line)
    {
        if (!line.contains(' '))
        {
            std::vector<std::optional<size_t>> numMatchesForCharsRemaining(line.length());
            const size_t permutations = match(trie, std::string_view(line), numMatchesForCharsRemaining);
            totalPermutations += permutations;
            numValid += permutations ? 1 : 0;
        }
    }

    std::println("Total valid designs: {}", numValid);
    std::println("Total valid permutations: {}", totalPermutations);
}
