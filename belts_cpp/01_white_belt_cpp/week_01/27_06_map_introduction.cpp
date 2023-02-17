#include <iostream>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

using namespace std::literals;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    bool isFirst = true;
    for (const auto& elm : v) {
        if (isFirst) {
            os << elm;
            isFirst = false;
        } else {
            os << " "s << elm;
        }
    }

    return os;
}

template <typename T, typename U>
void PrintMap(const std::map<T, U>& m) {
    std::cout << "map size = " << m.size() << std::endl;
    for (const auto& [key, value] : m) {
        std::cout << key << ": " << value << std::endl;
    }
}

template <typename T, typename U>
auto BuildReverseMap(const std::map<T, U>& dict) {
    std::map<U, T> reverted_dict;
    for (const auto& [key, value] : dict) {
        reverted_dict[value] = key;
    }

    return reverted_dict;
}

int main() {
    std::map<int, std::string> m = {{1, "odd"}, {2, "even"}, {1, "one"}};
    // m[2] = "two";
    // m.erase(1);
    // m[3] = "three";
    // m.erase(4);
    // m[5] = "five";
    std::cout << m.size() << std::endl;

    return 0;
}
