#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>


// Эта поразрядная сортировка (radix sort) работает ТОЛЬКО для БЕЗЗНАКОВЫХ ЧИСЕЛ (за O(N))
template <typename TValue, typename = std::enable_if_t<std::is_unsigned_v<TValue>>>
void RadixSort(std::vector<TValue>& data) {
    for (auto i = 0U; i < sizeof(TValue); ++i) {
        std::array<std::vector<size_t>, 256> count;
        TValue mask = 255;
        mask <<= (i * 8);

        for (auto j = 0ULL; j < data.size(); ++j) {
            auto index = (data[j] & mask) >> (i * 8);
            count[index].push_back(j);
        }

        std::vector<TValue> res;
        res.reserve(data.size());

        for (const auto& indexes : count) {
            for (auto index : indexes) {
                res.push_back(data[index]);
            }
        }

        std::swap(data, res);
    }
}

int main() {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<unsigned int> dis;

    std::vector<unsigned int> data;
    data.reserve(10000);

    for (auto i = 0; i < 10000; ++i) {
        data.push_back(dis(gen));
    }

    std::cout << std::boolalpha << std::is_sorted(data.begin(), data.end()) << std::endl;

    RadixSort(data);

    std::cout << std::boolalpha << std::is_sorted(data.begin(), data.end()) << std::endl;
}
