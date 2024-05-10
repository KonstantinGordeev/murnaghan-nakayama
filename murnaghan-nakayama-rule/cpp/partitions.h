#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

#ifndef CPP_PARTITIONS_H
#define CPP_PARTITIONS_H

template<typename T = size_t>
class Partition {
    using Container = std::vector<T>;
public:
    explicit Partition(): _data({}) {};

    explicit Partition(const std::vector<T>& data, bool needs_correction=true) {
        if (!needs_correction) {
            _data = data;
            return;
        }
        std::copy_if(data.begin(), data.end(), std::back_inserter(_data),
                     [](int i) { return i > 0; });
        std::sort(_data.rbegin(), _data.rend());
    };

    Container::iterator begin() {
        return _data.begin();
    }

    Container::iterator end() {
        return _data.end();
    }

    [[nodiscard]] Container::const_iterator begin() const {
        return _data.begin();
    }

    [[nodiscard]] Container::const_iterator end() const {
        return _data.end();
    }

    Container::reverse_iterator rbegin() {
        return _data.rbegin();
    }

    Container::reverse_iterator rend() {
        return _data.rend();
    }

    [[nodiscard]] Container::const_reverse_iterator rbegin() const {
        return _data.rbegin();
    }

    [[nodiscard]] Container::const_reverse_iterator rend() const {
        return _data.rend();
    }

    T& operator[](T index) {
        return _data[index];
    }

    const T& operator[](T index) const {
        return _data[index];
    }

    [[nodiscard]] size_t size() const {
        return _data.size();
    }

    [[nodiscard]] size_t num() const {
        return std::accumulate(_data.begin(), _data.end(), 0);
    }

    std::vector<T>& data() {
        return _data;
    }

    [[nodiscard]] const std::vector<T>& data() const {
        return _data;
    }

    void emplace_back(T item) {
        _data.emplace_back(item);
    }

private:
    std::vector<T> _data;
};

template<typename T>
bool operator==(const Partition<T>& lhs, const Partition<T>& rhs) {
    return lhs.data() == rhs.data();
}

template<typename T>
bool operator<(const Partition<T>& lhs, const Partition<T>& rhs) {
    size_t partition_len = std::min(lhs.size(), rhs.size());

    // lexicographic order
    for (size_t index = 0; index < partition_len; ++index) {
        if (lhs[index] > rhs[index]) {
            return false;
        } else if (lhs[index] < rhs[index]) {
            return true;
        }
    }
    return lhs.size() < rhs.size();

    // dominance partial order
//    size_t lhs_cumulative_sum = 0, rhs_cumulative_sum = 0;
//
//    for (size_t index = 0; index < partition_len; ++index) {
//        lhs_cumulative_sum += lhs[index];
//        rhs_cumulative_sum += rhs[index];
//        if (lhs_cumulative_sum > rhs_cumulative_sum) {
//            return false;
//        }
//    }
//    return true;
}

template<typename T = size_t>
class PartitionsCalculator {
public:
    // Dynamically calculates partitions for every k in interval [_partitions.size() + 1, n]
    // Values caches in '_partitions'
    // Returns vector of partitions for n
    const std::vector<Partition<T>>& get(size_t n) {
        for (auto num = _partitions.size(); num < n + 1; ++num) {
            _calculate(num);
        }
        return _partitions[n];
    }

private:
    void _calculate(size_t num) {
        std::vector<Partition<T>> partitions_of_num;
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
    std::vector<std::vector<Partition<T>>> _partitions = {{Partition()}, {Partition({1})}};
};

template<typename T = size_t>
struct PartitionHash {
    //  https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933
    static uint64_t fnv_64_buf(Partition<T> const& lambda) {
        std::size_t seed = lambda.size();
        for(const auto& item: lambda) {
            seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    size_t operator()(Partition<> const& lambda) const noexcept{
        return fnv_64_buf(lambda);
    }
};

#endif //CPP_PARTITIONS_H
