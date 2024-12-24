#include "aoc_days.h"
#include <vector>
#include <map>
#include <ranges>
#include <set>
#include <algorithm>

namespace
{
    struct Computer
    {
        std::string name;
        std::vector<std::shared_ptr<Computer>> links;
    };
}

void aoc::day23(std::string_view inputFilePath)
{
    std::map<std::string, std::shared_ptr<Computer>> computers;
    std::vector<std::string> allNames;
    std::ifstream file(inputFilePath.data());
    for (std::string line; std::getline(file, line);)
    {
        static auto addOrGet = [](std::map<std::string, std::shared_ptr<Computer>>& computers, const std::string& name) -> std::shared_ptr<Computer>
            {
                auto existing = computers.find(name);
                if (existing == computers.end())
                {
                    auto computer = std::shared_ptr<Computer>(new Computer{ .name = name });
                    existing = computers.emplace(name, computer).first;
                }
                return existing->second;
            };

        auto computer1 = addOrGet(computers, std::string(line.data(), 2));
        auto computer2 = addOrGet(computers, std::string(line.data() + 3, 2));
        computer1->links.emplace_back(computer2);
        computer2->links.emplace_back(computer1);
    }

    for (const auto pair : computers)
        std::sort(pair.second->links.begin(), pair.second->links.end());

    size_t numSetsOfThreeWithT = 0;

    std::set<std::vector<Computer*>> visitedLinkedSets;
    for (const auto& computer : computers)
    {
        for (const auto other : computer.second->links)
        {
            std::vector<std::shared_ptr<Computer>> intersecting;
            std::set_intersection(computer.second->links.begin(), computer.second->links.end(), other->links.begin(), other->links.end(), std::back_inserter(intersecting));

            for (const auto intersected : intersecting)
            {
                std::vector<Computer*> set;
                set.push_back(computer.second.get());
                set.push_back(other.get());
                set.push_back(intersected.get());
                std::sort(set.begin(), set.end());
                visitedLinkedSets.emplace(set);
            }
        }
    }

    for (const auto& set : visitedLinkedSets)
        for (const auto& computer : set)
            if (computer->name.starts_with('t'))
            {
                numSetsOfThreeWithT += 1;
                break;
            }


    std::vector<std::shared_ptr<Computer>> largestSet;
    size_t largestSize = 0;

    for (const auto& computer : computers)
    {
        std::vector<std::shared_ptr<Computer>> set1;
        set1.push_back(computer.second);
        set1.append_range(computer.second->links);
        std::sort(set1.begin(), set1.end());


        std::vector<std::vector<std::shared_ptr<Computer>>> intersections;

        for (const auto other : computer.second->links)
        {
            std::vector<std::shared_ptr<Computer>> set2;
            set2.push_back(other);
            set2.append_range(other->links);
            std::sort(set2.begin(), set2.end());

            intersections.push_back({});
            auto& intersection = intersections.back();
            std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(intersection));
        }

        size_t checkedSize = set1.size();
        std::vector<std::shared_ptr<Computer>> lastMatched;
        while (checkedSize > 1)
        {
            size_t matchedCount = 0;
            for (const auto& intersection : intersections)
            {
                if (intersection.size() == checkedSize)
                {
                    ++matchedCount;
                    lastMatched = intersection;
                }
            }

            if (matchedCount == checkedSize - 1)
            {
                if (matchedCount > largestSize)
                {
                    largestSet = lastMatched;
                    largestSize = matchedCount;
                }
            }
            --checkedSize;
        }
    }


    std::vector<std::string> sortedSet;
    for (const auto& c : largestSet)
        sortedSet.emplace_back(c->name);
    std::sort(sortedSet.begin(), sortedSet.end());

    std::println("Number of sets of 3 that contain a start with t: {}", numSetsOfThreeWithT);

    std::cout << "Largest LAN: ";
    for (int i = 0; i < sortedSet.size(); ++i)
        std::cout << sortedSet[i] << ((i != sortedSet.size() - 1) ? "," : "");
    std::cout << std::endl;
}
