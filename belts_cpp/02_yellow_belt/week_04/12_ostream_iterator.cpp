#include <algorithm>
#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <ostream>
#include <sstream>  // std::ostringstream
#include <string>
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
    return os << "["s << Join(v, ", "s) << "]"s;
}

// пишем свой алгоритм "copy"
template <typename InputIt, typename OutputIt>
OutputIt MyCopy(InputIt first, InputIt last,
                OutputIt d_first) {
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}

int main() {
    std::vector<int> v = {3, 5, 8, 12};

    // std::vector<int> v_copy(v.size());
    // MyCopy(v.begin(), v.end(), v_copy.begin());

    // // выводим содержимое вектора в консоль
    // std::cout << v_copy << std::endl;

    // также для вывода в консоль можно воспользоваться алгоритмом "MyCopy" и итератором "ostream_iterator<T>"
    // MyCopy(cbegin(v), cend(v), std::ostream_iterator<int>{std::cout, ", "});
    // std::cout << std::endl;

    // также для вывода в консоль можно воспользоваться стандартным алгоритмом "std::copy" и итератором "ostream_iterator<T>"
    std::copy(cbegin(v), cend(v), std::ostream_iterator<int>{std::cout, ", "});
    std::cout << std::endl;

    return 0;
}
