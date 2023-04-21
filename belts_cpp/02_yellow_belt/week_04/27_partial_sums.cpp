#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <map>
#include <numeric>
#include <sstream>
#include <utility>  // std::pair
#include <vector>   // std::partial_sum

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
    return os << Join(v);
}

double myfunc(const std::pair<const int, double>& lhs, const std::pair<const int, double>& rhs) {
    return lhs.second + rhs.second;
}

int main() {
    std::vector<int> data{2, 3, 5, 7, 11, 13, 17, 19};
    std::cout << "Partial sums: ";
    std::partial_sum(std::begin(data), std::end(data), std::ostream_iterator<int>{std::cout, " "});
    std::cout << std::endl;  // Partial sums: 2 5 10 17 28 41 58 77

    return 0;
}
