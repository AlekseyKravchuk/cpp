#include <algorithm>
#include <iostream>
#include <numeric>  // std::iota
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

    // template< class ForwardIterator, class T >
    // void iota(ForwardIterator first, ForwardIterator last, T value);

    // Заполняет диапазон [first, last) последовательно возрастающими значениями, начиная с "value" и вычисляя каждое следующее значение как ++value.
    // Обратные итераторы позволяют заполнить его, двигаясь от конца к началу.
    std::iota(v.rbegin(), v.rend(), 1);

    do {
        std::cout << v << std::endl;
    } while (std::prev_permutation(v.begin(), v.end()));

    return 0;
}
