#include <algorithm>
#include <iterator>
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

    Container::iterator rbegin() {
        return _data.rbegin();
    }

    Container::iterator rend() {
        return _data.rend();
    }

    T& operator[](T index) {
        return _data[index];
    }

    const T& operator[](T index) const {
        return _data[index];
    }

    size_t size() const {
        return _data.size();
    }

    std::vector<T>& data() {
        return data;
    }

    const std::vector<T>& data() const {
        return data;
    }

private:
    std::vector<T> _data;
};

template<typename T = size_t>
class PartitionsCalculator {
public:
    std::vector<Partition<T>> get(size_t n) {
        for (auto num = _partitions.size(); num < n + 1; ++num) {
            _calculate(num);
        }
        return _partitions[n];
    }
private:
    void _calculate(size_t num) {
        std::vector<Partition<>> partitions_of_num;
        for (size_t index = 0; index < num; ++index) {
            for (auto partition: _partitions[index]) {
                partition.data().emplace_back(num - index);
                std::sort(partition.rbegin(), partition.rend());
                partitions_of_num.push_back(partition);
            }
        }
        std::sort(partitions_of_num.data()->begin(), partitions_of_num.data()->end());
        auto last = std::unique(partitions_of_num.data()->begin(), partitions_of_num.data()->end());
        partitions_of_num.erase(last, partitions_of_num.end());
        _partitions.push_back(partitions_of_num);
    };
    std::vector<std::vector<Partition<>>> _partitions;
};

template<typename T = size_t>
struct PartitionHash {
    //  https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933
    static uint64_t fnv_64_buf(Partition<T> const& lambda) {
        std::size_t seed = lambda.size();
        for(auto& item: lambda) {
            seed ^= item + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    size_t operator()(Partition<> const& lambda) const noexcept{
        return fnv_64_buf(lambda);
    }
};

#endif //CPP_PARTITIONS_H
