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
    // пусть имеется некоторый вектор строк:
    std::vector<std::string> words{"one"s, "two"s, "three"s};

    // требуется сгруппировать слова по первой букве:
    std::map<char, std::vector<std::string>> grouped_words;

    for (const auto& word : words) {
        // от одного такого обращения: "grouped_words[word[0]]"
        // в словарь будет добавлен ключ "word[0]", которому будет сопоставлен ПУСТОЙ ВЕКТОР
        grouped_words[word[0]].push_back(word);
    }

    PrintMap(grouped_words);

    return 0;
}
