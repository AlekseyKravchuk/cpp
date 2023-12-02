#include <algorithm>
// #include <fstream>
#include <iostream>
// #include <map>
// #include <set>
#include <cctype>  // std::tolower
#include <cmath>
#include <vector>

using namespace std::literals;

/*
Задание по программированию
«Сортировка без учёта регистра»

Условие:
В стандартном потоке дана одна строка, состоящая из числа N и следующих за ним N строк S. Между собой число и строки разделены пробелом.
Отсортируйте строки S в лексикографическом порядке по возрастанию, игнорируя регистр букв, и выведите их в стандартный поток вывода.

Ограничения:
    0 <= N <= 1000
    1 <= |S| <= 15
    Каждая строка S[i] может состоять из следующих символов: [0-9,a-z,A-Z]

Примеры:

stdin: 2 q A => stdout: A q
stdin: 3 a C b => stdout: a b C

Подсказка:
функция sort дополнительно принимает третий аргумент. Изучите его подробнее по ссылке.

*/

template <typename Collection>
void PrintCollection(const Collection& collection, const std::string& message = ""s) {
    if (!message.empty()) {
        std::cout << message;
    }

    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << " "s << elm;
        }
    }
    std::cout << std::endl;
}

int main() {
    int n{};
    std::cin >> n;
    std::vector<std::string> v_strings;
    std::string str;

    for (int i = 0; i < n; ++i) {
        std::cin >> str;
        v_strings.push_back(str);
    }

    // // 1-ый вариант сортировки
    // std::sort(v_strings.begin(),
    //           v_strings.end(),
    //           [](const auto& s1, const auto& s2) {
    //               std::string lowered_1{}, lowered_2{};

    //               std::transform(s1.begin(),
    //                              s1.end(),
    //                              std::inserter(lowered_1, lowered_1.begin()),
    //                              [](unsigned char ch) {
    //                                  return std::tolower(ch);
    //                              });

    //               std::transform(s2.begin(),
    //                              s2.end(),
    //                              std::inserter(lowered_2, lowered_2.begin()),
    //                              [](unsigned char ch) {
    //                                  return std::tolower(ch);
    //                              });
    //             return lowered_1 < lowered_2;
    //           });

    // 2-ой вариант сортировки
    std::sort(v_strings.begin(),
              v_strings.end(),
              [](const std::string& s1, const std::string& s2) {
                  return std::lexicographical_compare(s1.begin(), s1.end(),
                                                      s2.begin(), s2.end(),
                                                      [](char ch1, char ch2) {
                                                          return std::tolower(ch1) < std::tolower(ch2);
                                                      });
              });

    PrintCollection(v_strings);

    return 0;
}
