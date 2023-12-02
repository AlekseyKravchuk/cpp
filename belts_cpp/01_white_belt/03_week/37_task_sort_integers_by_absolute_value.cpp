#include <algorithm>
// #include <fstream>
#include <iostream>
// #include <map>
// #include <set>
#include <cmath>
#include <vector>

using namespace std::literals;

/*
Задание по программированию
«Сортировка целых чисел по модулю»

Условие:
В стандартном потоке дана одна строка, состоящая из N + 1 целых чисел.
Первым числом идёт само число N. Далее следуют ещё N чисел, обозначим их за массив A. Между собой числа разделены пробелом.
Отсортируйте массив А по модулю и выведите его в стандартный поток.

Ограничения:
    0 <= N <= 1000
    -1000000 <= A[i] <= 1000000

Примеры

stdin: 2 -4 3 => stdout: 3 -4
stdin: 3 1 -3 2 => stdout: 1 2 -3

Подсказка

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
    std::vector<int> v;
    int num;

    for (int i = 0; i < n; ++i) {
        std::cin >> num;
        v.push_back(num);
    }

    std::sort(v.begin(), v.end(), [](const int lhs, const int rhs) {
        return std::abs(lhs) < std::abs(rhs);
    });

    PrintCollection(v);

    return 0;
}
