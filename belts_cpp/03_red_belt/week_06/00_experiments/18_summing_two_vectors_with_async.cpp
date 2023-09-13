#include <algorithm>
#include <future>
#include <iostream>
#include <numeric> // std::accumulate, std::iota
#include <vector>

#include "../../log_duration.h"

// однопоточная версия
uint64_t SumTwoVectorsSingleThread(const std::vector<uint64_t>& v1, const std::vector<uint64_t>& v2) {
    return std::accumulate(v1.begin(), v1.end(), 0) +
           std::accumulate(v2.begin(), v2.end(), 0);
}

// многопоточная версия
uint64_t SumTwoVectorsMultiThread(const std::vector<uint64_t>& v1, const std::vector<uint64_t>& v2) {
    std::future<uint64_t> f = std::async([&v1]() {
        return std::accumulate(v1.begin(), v1.end(), static_cast<uint64_t>(0));
    });

    uint64_t result = std::accumulate(v2.begin(), v2.end(), 0);

    return result + f.get();
}

int main() {
    // std::vector<int> v = {1, 2, 3, 4, 5};
    // std::cout << std::accumulate(v.begin(), v.end(), 0) << std::endl;

    // std::vector<int> v1 = {1, 2, 3, 4, 5};
    // std::vector<int> v2 = {10, 20, 30, 40, 50};

    std::vector<uint64_t> v1(1'000'000, 1);
    std::vector<uint64_t> v2(1'000'000, 1);

    {
        LOG_DURATION("Single thread summing vectors");
        std::cout << SumTwoVectorsSingleThread(v1, v2) << std::endl;
    }

    // {
    //     LOG_DURATION("Multi thread summing vectors");
    //     std::cout << SumTwoVectorsMultiThread(v1, v2) << std::endl;
    // }

    return 0;
}
