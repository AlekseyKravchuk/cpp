#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

using namespace std::literals;

// нужно переопределить оператор вывода (stream insertion operator << ) для пары std::pair
template <typename Iterator>
std::ostream& operator<<(std::ostream& os, const std::pair<Iterator, Iterator>& pair) {
    os << "<"s << *pair.first << ", " << *pair.second << ">"s;
    return os;
}

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    std::vector<int> numbers = {22, 49, 18, 120, 14, 3, 22, 26, 11, 100, 4};

    auto itMin = std::min_element(numbers.begin(), numbers.end());
    auto itMax = std::max_element(numbers.begin(), numbers.end());

    // также есть алгоритм, возврадающий ПАРУ <min, max>
    auto itMinMax = std::minmax_element(numbers.begin(), numbers.end());

    // проверяем, что получилось
    std::cout << "Minimum element in vector: ";
    PrintRange(numbers.begin(), numbers.end());
    std::cout << *itMin << std::endl;

    std::cout << std::endl;

    std::cout << "Maximum element in vector: ";
    PrintRange(numbers.begin(), numbers.end());
    std::cout << *itMax << std::endl;

    std::cout << std::endl;

    // // для этого случая нужно иметь перегруженный "stream insertion operator <<" для std::pair<Iterator, Iterator>
    std::cout << "Pair of <min, max> elements in vector: ";
    PrintRange(numbers.begin(), numbers.end());
    // std::cout << *itMinMax.first << " "s << *itMinMax.second << std::endl;
    std::cout << itMinMax << std::endl;

    return 0;
}