#include <algorithm>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std::literals;

/*
=============== Перебор перестановок ===============
Дано целое положительное число N, не превышающее 9.
Выведите все перестановки чисел от 1 до N в обратном лексикографическом порядке (см. пример).
Пример
Ввод:
3

Вывод:
3 2 1
3 1 2
2 3 1
2 1 3
1 3 2
1 2 3
*/

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
    return os << Join(v);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = n - i;
    }

    do {
        std::cout << v << std::endl;
    } while (std::prev_permutation(v.begin(), v.end()));

    return 0;
}
