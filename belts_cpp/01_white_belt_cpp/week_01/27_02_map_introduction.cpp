#include <iostream>
#include <map>
#include <utility>

using namespace std::literals;

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
    std::map<std::string, int> m = {
        {"one"s, 1},
        {"two"s, 2},
        {"three"s, 3},
        {"four"s, 4}
    };

    PrintMap(m);

    m.erase("two");

    auto rm = BuildReverseMap(m);
    PrintMap(rm);

    return 0;
}
