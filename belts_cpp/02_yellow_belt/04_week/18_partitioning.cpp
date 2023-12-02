// C++ code to demonstrate the working of partition() and is_partitioned()
#include <algorithm>  // for partition algorithm
#include <iostream>
#include <sstream>
#include <vector>

using namespace std::literals;

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = " "s) {
    bool isFirst = true;
    std::ostringstream oss;

    for (const auto& elm : collection) {
        if (!isFirst) {
            oss << delimeter;
        }
        isFirst = false;
        oss << elm;
    }
    return oss.str();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    return os << "["s << Join(v) << "]"s;
}

int main() {
    std::vector<int> v = {2, 1, 5, 6, 8, 7};
    std::cout << "Initial state of vector 'v': " << v << std::endl;

    // Checking if vector is partitioned using is_partitioned()
    bool isPartitioned = is_partitioned(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
    });

    std::cout << ((isPartitioned) ? "Vector is partitioned"s : "Vector is not partitioned"s) << std::endl;

    // partitioning vector using partition()
    auto it = std::partition(v.begin(), v.end(),
                   [](int x) {
                       return x % 2 == 0;
                   });
    std::cout << "State of vector 'v' after partitioning on 'even' and 'odd' parts: " << v << std::endl;

    v.erase(it, v.end());
    std::cout << "State of vector 'v' after erasing of second partitioned part: " << v << std::endl;

    return 0;
}
