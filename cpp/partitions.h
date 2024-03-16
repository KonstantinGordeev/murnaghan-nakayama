#include <algorithm>
#include <iterator>
#include <vector>

#ifndef CPP_PARTITIONS_H
#define CPP_PARTITIONS_H

class Partition {
public:
    explicit Partition(): _data({}) {};

    explicit Partition(const std::vector<size_t>& data, bool needs_correction=true) {
        if (!needs_correction) {
            _data = data;
            return;
        }
        std::copy_if(data.begin(), data.end(), std::back_inserter(_data),
                     [](int i) { return i > 0; });
        std::sort(_data.rbegin(), _data.rend());
    };

    auto begin() {
        return _data.begin();
    }

    auto end() {
        return _data.end();
    }

private:
    std::vector<size_t> _data;
};

class PartitionsCalculator {
public:
    std::vector<Partition> get(size_t n) {
        if
    }
private:
    void _calculate(size_t num) {
        std::vector<Partition> partitions_of_num;
        for (size_t index = 0; index < num; ++index) {
            for (auto partition: _partitions[index]) {
                partition.emplace_back(num - index);
                std::sort(partition.rbegin(), partition.rend());
                partitions_of_num.push_back(partition);
            }
        }
        std::sort(partitions_of_num.begin(), partitions_of_num.end());
        auto last = std::unique(partitions_of_num.begin(), partitions_of_num.end());
        partitions_of_num.erase(last, partitions_of_num.end());
        _partitions.push_back(partitions_of_num);
    };
    std::vector<std::vector<Partition>> _partitions;
};

#endif //CPP_PARTITIONS_H
