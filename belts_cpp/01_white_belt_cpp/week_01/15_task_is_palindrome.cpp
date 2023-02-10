#include <ios>  // std::boolalpha
#include <iostream>
#include <string>

using namespace std::literals;

/*
Напишите функцию, которая:
    - называется IsPalindrom
    - возвращает bool
    - принимает параметр типа string и возвращает, является ли переданная строка палиндромом

Палиндром - это слово или фраза, которые одинаково читаются слева направо и справа налево.
Пустая строка является палиндромом.
 */

// ============== Yandex solution ==============
// bool IsPalindrom(std::string s) {
//     // Замечание: более правильным было бы использовать здесь тип size_t вместо int
//     // О причинах Вы узнаете на Жёлтом поясе
//     for (int i = 0; i < s.size() / 2; ++i) {
//         if (s[i] != s[s.size() - i - 1]) {
//             return false;
//         }
//     }
//     return true;
// }

bool IsPalindrom(std::string s) {
    for (int i = 0, j = static_cast<int>(s.size() - 1); i <= j; ++i, --j) {
        if (s[i] != s[j]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::string s{};
    std::cin >> s;

    std::cout << std::boolalpha << IsPalindrom(s);

    return 0;
}
