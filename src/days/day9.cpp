#include "aoc_days.h"

#include <vector>
#include <ranges>

namespace
{
    std::span<int> findSpace(std::vector<int>& diskLayout, size_t spaceRequired, int& searchStartIndex, int searchStopIndex)
    {
        for (int i = 0, l1 = searchStartIndex, l2 = l1; l2 < searchStopIndex; l1 = l2 + 1, ++i)
        {
            while (l1 < diskLayout.size() && diskLayout[l1] != -1) ++l1;
            l2 = l1;
            while (l2 < diskLayout.size() - 1 && diskLayout[l2 + 1] == -1) ++l2;
            if (i == 0)
                searchStartIndex = l1;

            if ((l2 - l1) + 1 >= spaceRequired && (l2 < searchStopIndex) && l2 < diskLayout.size())
                return std::span<int>(diskLayout.begin() + l1, diskLayout.begin() + l1 + spaceRequired);
        }
        return {};
    }

    size_t compact(std::vector<int>&& diskLayout, bool allowFragmentation)
    {
        int lStart = 0;
        for (int r1 = -1, r2 = (int)diskLayout.size() - 1; r2 >= 0; r2 = r1 - 1)
        {
            while (r2 >= 0 && diskLayout[r2] == -1) --r2;
            r1 = r2;
            while (!allowFragmentation && (r1 > 0 && diskLayout[r1 - 1] == diskLayout[r2])) --r1;
            const size_t spaceRequired = (r2 - r1) + 1;

            if (std::span<int> space = findSpace(diskLayout, spaceRequired, lStart, r1); !space.empty())
            {
                std::fill(space.begin(), space.end(), diskLayout[r1]);
                std::fill(diskLayout.begin() + r1, diskLayout.begin() + (r1 + spaceRequired), -1);
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
