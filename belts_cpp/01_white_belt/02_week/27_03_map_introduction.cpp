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
    std::vector<std::string> words = {"one"s, "two"s, "three"s, "four"s, "two"s, "one"s, "two"s, "two"s};
    
    // требуется посчитать частоту встречаемости каждого слова в векторе "words"
    std::map<std::string, int> counter;
    for (const auto& word : words) {
        // Что происходит в результате обращения к конкретному ключу с помощью квадратных скобок?
        // Если происходит обращение по некоторому ключу И этот ключ имеется в словаре, то компилятор возвращает ССЫЛКУ на значение по этому ключу
        // Если же указанный ключ НЕ СУЩЕСТВУЕТ, то компилятор добавляет его и сопоставляет ему некоторое значение по умолчанию (для int - ноль)
        ++counter[word];
    }

    PrintMap(counter);

    return 0;
}
