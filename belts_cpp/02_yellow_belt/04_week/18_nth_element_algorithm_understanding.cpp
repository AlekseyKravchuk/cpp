// C++ program to demonstrate the use of std::nth_element
#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>

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

//post-condition: After returning, the elements in v may be reordered and the resulting order is implementation defined.
double median(std::vector<double>& v) {
    if (v.empty()) {
        return 0.0;
    }
    auto i = v.size() / 2;
    nth_element(v.begin(), v.begin() + i, v.end());
    auto med = v[i];

    if ((v.size() % 2) == 0) {  //If the set size is even
        auto max_it = max_element(v.begin(), v.begin() + i);
        med = (*max_it + med) / 2.0;
    }
    return med;
}

int main() {
    // в отсортированном виде вектор имеет вид: [2 3 10 23 33 45 47 56 88], size: 9
    std::vector<int> v = {88, 33, 3, 2, 10, 45, 56, 23, 47};
    size_t i = v.size() / 2;
    std::nth_element(v.begin(), v.begin() + i, v.end());
    std::cout << "vector 'v' after processing by nth_element algoritm: "s << v << std::endl;

    // вектор с ЧЕТНЫМ количеством элементов
    // в отсортированном виде вектор имеет вид: [2 3 10 33 45 47 56 88], size: 8
    std::vector<int> k = {88, 33, 3, 2, 10, 45, 56, 47};
    size_t j = k.size() / 2;
    std::nth_element(k.begin(), k.begin() + j, k.end());
    std::cout << "vector 'k' after processing by nth_element algoritm: "s << k << std::endl;
}
