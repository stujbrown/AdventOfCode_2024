#include "aoc_days.h"
#include <vector>
#include <map>
#include <ranges>
#include <array>
#include <sstream>
#include <optional>

namespace
{
    struct Gate;
    struct Wire
    {
        std::string name;
        std::shared_ptr<Gate> gateInput;
        int priority = -1;
        std::optional<bool> value;
    };

    struct Gate
    {
        enum class Type { And, Or, Xor } type;
        std::array<std::shared_ptr<Wire>, 2> inputs;
    };

    std::optional<bool> evaluate(const Wire* wire);
    std::optional<bool> evaluate(const Gate* gate)
    {
        std::optional<bool> a = evaluate(gate->inputs[0].get());
        std::optional<bool> b = evaluate(gate->inputs[1].get());
        if (!a.has_value() || !b.has_value())
            return {};

        if (gate->type == Gate::Type::And)
            return *a & *b;
        else if (gate->type == Gate::Type::Or)
            return *a | *b;
        return *a ^ *b;
    }

    std::optional<bool> evaluate(const Wire* wire)
    {
        if (wire->value.has_value())
            return *wire->value;

        std::optional<bool> val = evaluate(wire->gateInput.get());
        if (val.has_value())
            return *val;

        return {};
    }
}

void aoc::day24(std::string_view inputFilePath)
{
    std::map<std::string, std::shared_ptr<Wire>> wires;
    std::array<Wire*, 64> bitWires[3];
    bitWires[0].fill(nullptr);
    bitWires[1].fill(nullptr);
    bitWires[2].fill(nullptr);

    auto getOrCreateWire = [&wires, &bitWires](const std::string& name, int priority = -1, std::optional<bool> value = {}) -> std::shared_ptr<Wire>
        {
            auto wireItr = wires.find(name);
            if (wireItr == wires.end())
            {
                auto parseIndex = [](const std::string& name) -> int {
                    int index = 0;
                    std::from_chars(name.data() + 1, name.data() + 3, index);
                    return index;
                    };

                wireItr = wires.emplace(name, std::shared_ptr<Wire>(new Wire{ .name = name, .priority = priority, .value = value })).first;
                if (name.starts_with("x"))
                    bitWires[0][parseIndex(name)] = wireItr->second.get();
                if (name.starts_with("y"))
                    bitWires[1][parseIndex(name)] = wireItr->second.get();
                if (name.starts_with("z"))
                    bitWires[2][parseIndex(name)] = wireItr->second.get();
            }
            return wireItr->second;
        };

    std::ifstream file(inputFilePath.data());
    int priority = 0;

    std::vector<std::pair<std::string, bool>> initSequence;
    for (std::string line; std::getline(file, line) && line.find_first_not_of(' ') != std::string::npos; ++priority)
    {
        auto wireValueSplit = (line | std::views::split(' ')).begin();
        std::string wireName((*wireValueSplit).begin(), (*wireValueSplit).end() - 1);
        ++wireValueSplit;
        std::string wireValue((*wireValueSplit).begin(), (*wireValueSplit).end());

        initSequence.emplace_back(wireName, wireValue[0] == '1' ? true : false);
        wires.emplace(wireName, std::shared_ptr<Wire>(new Wire{ .name = wireName, .priority = priority,
            .value = wireValue[0] == '1' ? true : false }));
    }

    for (std::string line; std::getline(file, line);)
    {
        std::string wireAName, op, wireBName, outputName;
        std::stringstream strStream(line);
        strStream >> wireAName >> op >> wireBName >> outputName >> outputName;

        auto wireA = getOrCreateWire(wireAName);
        auto wireB = getOrCreateWire(wireBName);
        auto wireOut = getOrCreateWire(outputName);

        if (wireOut->gateInput.get() != nullptr)
            throw std::exception("More than one input for wire");

        Gate::Type type = Gate::Type::And;
        if (op[0] == 'X') type = Gate::Type::Xor;
        else if (op[0] == 'O') type = Gate::Type::Or;
        auto newGate = std::shared_ptr<Gate>(new Gate{ .type = type });
        newGate->inputs[0] = wireA;
        newGate->inputs[1] = wireB;
        wireOut->gateInput = newGate;
    }

    uint64_t value = 0;
    for (int i = 0; bitWires[2][i] != nullptr; ++i)
    {
        const auto v = evaluate(bitWires[2][i]);
        if (v.has_value())
            value |= (uint64_t)((*v ? 1 : 0)) << i;
    }

    std::println("Z output value: {}", value);
}

