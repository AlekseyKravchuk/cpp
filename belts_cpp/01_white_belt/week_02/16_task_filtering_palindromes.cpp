#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Отбор палиндромов»
Напишите функцию, которая
    - называется PalindromFilter
    - возвращает vector<string>
    - принимает vector<string> words и int minLength и возвращает все строки из вектора words, которые являются палиндромами
    и имеют длину не меньше minLength

Входной вектор содержит не более 100 строк, длина каждой строки не больше 100 символов.
 */

void PrintWords(const std::vector<std::string>& v) {
    bool isFirst = true;

    for (const auto& word : v) {
        if (isFirst) {
            std::cout << word;
            isFirst = false;
        } else {
            std::cout << ' ' << word;
        }
    }
    std::cout << std::endl;
}

bool IsPalindrom(std::string s) {
    for (int i = 0, j = static_cast<int>(s.size() - 1); i <= j; ++i, --j) {
        if (s[i] != s[j]) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> PalindromFilter(std::vector<std::string> words, int minLength) {
    std::vector<std::string> filtered;

    for (const auto& word : words) {
        if (word.size() >= minLength && IsPalindrom(word)) {
            filtered.push_back(word);
        }
    }

    return filtered;
}

int main() {
    {
        std::vector<std::string> words{"abacaba"s, "aba"s};
        int minLength = 5;
        std::vector<std::string> filtered = PalindromFilter(words, minLength);
        PrintWords(filtered);
    }

    {
        std::vector<std::string> words{"abacaba"s, "aba"s};
        int minLength = 2;
        std::vector<std::string> filtered = PalindromFilter(words, minLength);
        PrintWords(filtered);
    }

    {
        std::vector<std::string> words{"weew"s, "bro"s, "code"s};
        int minLength = 4;
        std::vector<std::string> filtered = PalindromFilter(words, minLength);
        PrintWords(filtered);
    }

    return 0;
}
