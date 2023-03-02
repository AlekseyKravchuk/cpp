#include <iostream>
#include <map>
#include <utility>
#include <vector>

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
    std::map<std::string, int> words = {
        {"one"s, 1},
        {"two"s, 2}};

    // при обращении по несуществующему ключу размер словаря увеличивается
    std::cout << words.size() << std::endl;

    auto tmp = words["three"s];
    std::cout << words.size() << std::endl;

    return 0;
}
