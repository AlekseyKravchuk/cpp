#include <algorithm>
#include <iostream>
#include <iterator>  // std::inserter
#include <map>
#include <set>
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
    return os << "["s << Join(v, ", "s) << "]"s;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s) {
    return os << "{"s << Join(s, ", "s) << "}"s;
}

template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os, const std::map<KeyType, ValueType>& m) {
    return os << "{"s << Join(m, ", "s) << "}"s;
}

int main() {
    // пусть требуется найти пересечение 2-х множеств
    std::set<int> a = {1, 8, 3};
    std::set<int> b = {3, 6, 8};

    std::set<int> target_set;
    std::set_intersection(a.begin(), a.end(),
                          b.begin(), b.end(),
                          std::inserter(target_set, target_set.begin()));
     std::cout << target_set << std::endl;

    // std::vector<int> target_vector;
    // std::set_intersection(a.begin(), a.end(),
    //                       b.begin(), b.end(),
    //                       std::back_inserter(target_vector));
    // std::cout << target_vector << std::endl;

    return 0;
}
