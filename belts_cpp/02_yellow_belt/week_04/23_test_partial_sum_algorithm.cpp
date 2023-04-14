#include <algorithm>
#include <iostream>
#include <numeric>  // std::partial_sum
#include <sstream>

using namespace std::literals;

template <typename Collection>
std::string Join(Collection collection, const std::string& delimeter = ", "s) {
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
    const unsigned int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    std::vector<int> partial_sums;
    std::partial_sum(std::begin(monthDays), std::end(monthDays),
                     std::back_inserter(partial_sums));
    std::cout << partial_sums << std::endl;
    return 0;
}
