#include <algorithm>
#include <unordered_map>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include "partitions.h"

using BigInt = boost::multiprecision::cpp_int;
using PartitionT = Partition<size_t>;
using PartitionHashT = PartitionHash<size_t>;

struct PartitionWithoutBorderStrip {
    PartitionT partition;
    std::vector<size_t> border_strip;
};

template<class Value>
BigInt MinusOnePow(Value power) {
    if (power % 2 == 0) {
        return 1;
    }
    return -1;
}

// Class for calculation the values of the character table of the finite symmetric groups
// The values are calculated using the Murnaghan–Nakayama rule
class CharTable {
private:
    std::vector<std::unordered_map<PartitionT, std::unordered_map<PartitionT, BigInt,
                PartitionHashT>, PartitionHashT>> character_values;
    PartitionsCalculator<size_t> partitions;


public:
    // Calculates character value of irreducible character on conjugacy class
    // lambda: irreducible character
    // rho: conjugacy class
    // return: value irreducible character of conjugacy class and save it into table
    BigInt char_value(const PartitionT& lambda, const PartitionT& rho) {
        size_t partition_len = lambda.num();
        if (character_values.size() < partition_len + 1) {
            character_values.resize(partition_len + 1);
        }

        if (partition_len < 2) {
            return 1;
        }

        // searching for a value in the cache
        auto value = character_values[partition_len][lambda].find(rho);
        if (value != character_values[partition_len][lambda].end()) {
            return value->second;
        }

        BigInt result = 0;
        auto border_strips = get_border_strips(lambda, rho[0]);
        for (const auto &pair: border_strips) {
            result += MinusOnePow(pair.border_strip.size() - 1) * char_value(pair.partition,
                                                              PartitionT({rho.begin() + 1, rho.end()}));
        }

        character_values[partition_len][lambda][rho] = result;

        return result;
    }

    // Calls char_value(...) for every irreducible character and conjugacy class
    //     of symmetric group of degree n
    // return: square table of character values
    std::vector<std::vector<BigInt>> character_table(size_t n) {
        std::vector<PartitionT> partitions_for_n = partitions.get(n);
        auto partitions_count = partitions_for_n.size();
        std::vector<std::vector<BigInt>> table(partitions_count, std::vector<BigInt>(partitions_count));

        for (size_t i_index = 0; i_index < partitions_count; ++i_index) {
            const auto &lambda = partitions_for_n.at(i_index);
            for (size_t j_index = 0; j_index < partitions_count; ++j_index) {
                const auto &rho = partitions_for_n.at(j_index);
                BigInt cv = char_value(lambda, rho);
                table[i_index][j_index] = cv;
            }
        }

        return table;
    }

// Return vector of sub-partition lambda* of lambda and border strip xi_ of sum bs_length such that
// lambda* + xi_ = lambda, lambda* is a correct partition and xi_ is connected and has no 2x2 parts
// Computational complexity is O(lambda[0] + len(lambda))
static std::vector<PartitionWithoutBorderStrip> get_border_strips(PartitionT lambda, BigInt bs_length) {
    std::vector<PartitionWithoutBorderStrip> result;
    size_t row = 0;
    std::vector<size_t> xi(lambda.size(), 0);
    size_t first_non_zero_row = 0;
    while (bs_length > 0 and row < lambda.size()) {
        if (row + 1 == lambda.size()) {  // last left step
            size_t step = std::min(static_cast<size_t>(bs_length), lambda[row]);
            bs_length -= step;
            lambda[row] -= step;
            xi[row] += step;
            if (bs_length > 0) {
                return result;
            }
        } else if (lambda[row] > lambda[row + 1]) {  // left step
            auto step = std::min(static_cast<size_t>(bs_length), lambda[row] - lambda[row + 1]);
            bs_length -= step;
            lambda[row] -= step;
            xi[row] += step;
        } else {  // down step
            auto prev_row = row;
            while (row < lambda.size() and lambda[row] == lambda[prev_row]) {
                bs_length -= 1;
                xi[row] += 1;
                row += 1;
            }

            for (auto index = prev_row; index < row; ++index) {
                lambda[index] -= 1;
            }

            row -= 1;

            while (bs_length < 0) {
                lambda[first_non_zero_row] += xi[first_non_zero_row];
                bs_length += xi[first_non_zero_row];
                xi[first_non_zero_row] = 0;
                first_non_zero_row += 1;
            }
        }

        if (bs_length == 0) {
            PartitionWithoutBorderStrip pair;
            pair.partition = PartitionT(lambda.data());
            std::copy_if(xi.begin(), xi.end(), std::back_inserter(pair.border_strip), [](int i) { return i > 0; });
            result.push_back(pair);

            lambda[first_non_zero_row] += xi[first_non_zero_row];
            bs_length += xi[first_non_zero_row];
            xi[first_non_zero_row] = 0;
            if (row == first_non_zero_row) {
                row += 1;
                first_non_zero_row += 1;
            }
        }
    }

    return result;
}
};
