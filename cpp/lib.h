#include <algorithm>
#include <unordered_map>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
//#include "partitions.h"

using BigInt = boost::multiprecision::cpp_int;
using Partition = std::vector<size_t>;

struct PartitionHash {
    size_t operator()(Partition const& lambda) const noexcept{
        size_t result = 0;
        for (size_t item: lambda) {
            result = result * 1024 + item;
        }
        return result;
    }
};

struct PartitionWithoutBorderStrip {
    Partition partition;
    std::vector<size_t> border_strip;
};

template<class Value>
Value Sum(const std::vector<Value>& vec) {
    Value result = 0;
    for (auto& item: vec) {
        result += item;
    }
    return result;
}

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
public:
    std::vector<std::unordered_map<Partition, std::unordered_map<Partition, BigInt,
                PartitionHash>, PartitionHash>> character_values;
    std::vector<std::vector<Partition>> partitions = {{{}}, {{1,}}};

    // Remove zero items from partition
    static Partition correct_partition(const Partition &lambda) {
        Partition new_lambda_;
        for (auto& item: lambda) {
            if (item > 0) {
                new_lambda_.push_back(item);
            }
        }
        return new_lambda_;
    }

    // Return vector of sub-partition lambda* of lambda and border strip xi_ of sum bs_length such that
    // lambda* + xi_ = lambda, lambda* is a correct partition and xi_ is connected and has no 2x2 parts
    // Computational complexity is O(lambda[0] + len(lambda))
    static std::vector<PartitionWithoutBorderStrip> get_border_strips(Partition lambda, BigInt bs_length) {
        std::vector<PartitionWithoutBorderStrip> result;
        size_t row = 0;
        Partition xi({lambda.size(), 0});
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
                result.push_back({lambda, xi});

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

    // Dynamically calculates partitions for every k in interval [len(self.partitions) + 1, n]
    // Values caches in 'self.partitions'
    // Returns vector of partitions for n
    std::vector<Partition>* calculate_partitions(size_t n) {
        for (auto num = partitions.size(); num < n + 1; ++num) {
            std::vector<Partition> partitions_of_num;
            for (size_t index = 0; index < num; ++index) {
                for (auto partition: partitions[index]) {
                    partition.emplace_back(num - index);
                    std::sort(partition.rbegin(), partition.rend());
                    partitions_of_num.push_back(partition);
                }
            }
            std::sort(partitions_of_num.begin(), partitions_of_num.end());
            auto last = std::unique(partitions_of_num.begin(), partitions_of_num.end());
            partitions_of_num.erase(last, partitions_of_num.end());
            partitions.push_back(partitions_of_num);
        }
        character_values.resize(n + 1);
        return &partitions[n];
    }

public:
    // Calculates character value of irreducible character on conjugacy class
    // lambda: irreducible character
    // rho: conjugacy class
    // return: value irreducible character of conjugacy class and save it into table
    BigInt char_value(Partition lambda, Partition rho) {
        lambda = correct_partition(lambda);
        rho = correct_partition(rho);
        size_t partition_len = Sum(lambda);

        if (partition_len < 2) {
            return 1;
        }

        auto value = character_values[partition_len][lambda].find(rho);
        if (value != character_values[partition_len][lambda].end()) {
            return value->second;
        }

        BigInt result = 0;
        auto border_strips = get_border_strips(lambda, rho[0]);
        for (const auto &pair: border_strips) {
            auto partition = correct_partition(pair.partition);
            auto xi = correct_partition(pair.border_strip);
            result += MinusOnePow(xi.size() - 1) * char_value(partition,
                                                              {rho.begin() + 1, rho.end()});
        }

        character_values[partition_len][lambda][rho] = result;

        return result;
    }

    // Calls char_value(...) for every irreducible character and conjugacy class
    //     of symmetric group of degree n
    // return: square table of character values
    std::vector<std::vector<BigInt>> character_table(size_t n) {
        auto partitions_for_n = calculate_partitions(n);
        auto partitions_count = partitions_for_n->size();
        std::vector<std::vector<BigInt>> table(partitions_count, std::vector<BigInt>(partitions_count));

        for (size_t i_index = 0; i_index < partitions_count; ++i_index) {
            const auto &lambda = partitions_for_n->at(i_index);
            for (size_t j_index = 0; j_index < partitions_count; ++j_index) {
                const auto &rho = partitions_for_n->at(j_index);
                BigInt cv = char_value(lambda, rho);
                table[i_index][j_index] = cv;
            }
        }

        return table;
    }
};
