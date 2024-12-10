#include "aoc_days.h"

#include <vector>
#include <ranges>

namespace
{
    struct Range 
    {
        int l, r;
        inline size_t length() const { return (r - l) + 1; }
    };

    std::span<int> findSpace(std::vector<int>& diskLayout, size_t spaceRequired, int& searchStartIndex, int searchStopIndex)
    {
        Range free{ searchStartIndex, searchStartIndex };
        for (int i = 0; free.r < searchStopIndex; free.l = free.r + 1, ++i)
        {
            while (free.l < diskLayout.size() && diskLayout[free.l] != -1) ++free.l;
            free.r = free.l;
            while (free.r < diskLayout.size() - 1 && diskLayout[free.r + 1] == -1) ++free.r;
            if (i == 0)
                searchStartIndex = free.l;

            if (free.length() >= spaceRequired && (free.r < searchStopIndex) && free.r < diskLayout.size())
                return std::span<int>(diskLayout.begin() + free.l, diskLayout.begin() + free.l + spaceRequired);
        }
        return {};
    }

    size_t compact(std::vector<int> diskLayout, bool allowFragmentation)
    {
        int lStart = 0;
        for (Range file{ -1, (int)diskLayout.size() - 1 }; file.r >= 0; file.r = file.l - 1)
        {
            while (file.r >= 0 && diskLayout[file.r] == -1) --file.r;
            file.l = file.r;
            while (!allowFragmentation && (file.l > 0 && diskLayout[file.l - 1] == diskLayout[file.r])) --file.l;

            if (std::span<int> space = findSpace(diskLayout, file.length(), lStart, file.l); !space.empty())
            {
                std::fill(space.begin(), space.end(), diskLayout[file.l]);
                std::fill(diskLayout.begin() + file.l, diskLayout.begin() + (file.l + file.length()), -1);
            }
        }

        size_t checksum = 0;
        for (size_t i = 0; i < diskLayout.size(); ++i)
            checksum += (diskLayout[i] != -1) ? diskLayout[i] * i : 0;
        return checksum;
    }
}

void aoc::day9(std::string_view inputFilePath)
{
    std::vector<int> diskLayout;
    std::ifstream file(inputFilePath.data());

    char c;
    for (int id = 0; file >> c; ++id)
    {
        diskLayout.append_range(std::views::repeat(id, int(c - '0')));
        diskLayout.append_range(std::views::repeat(-1, (file >> c) ? int(c - '0') : 0));
    }

    std::println("Compacted checksum: {}", compact(std::move(std::vector<int>(diskLayout)), true));
    std::println("Optimal checksum: {}", compact(std::move(std::vector<int>(diskLayout)), false));
}
