#include <iostream>
#include <map>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

/*
===================== Всё в квадрат =====================

Реализуйте шаблонную функцию Sqr, которая работает не только для чисел, но и для контейнеров.
Функция должна возвращать копию исходного контейнера, модифицировав его следующим образом:

    для vector элементы нужно возвести в квадрат;
    для map в квадрат нужно возвести только значения, но не ключи;
    для pair в квадрат нужно возвести каждый элемент пары.

/ Пример вызова функции
vector<int> v = {1, 2, 3};
cout << "vector:";
for (int x : Sqr(v)) {
  cout << ' ' << x;
}
cout << endl;

map<int, pair<int, int>> map_of_pairs = {
  {4, {2, 2}},
  {7, {4, 3}}
};
cout << "map of pairs:" << endl;
for (const auto& x : Sqr(map_of_pairs)) {
  cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
}

Код выше должен вывести

vector: 1 4 9
map of pairs:
4 4 4
7 16 9

Функция должна корректно работать не только для контейнеров, состоящих из чисел, но и для составных объектов, например, векторов словарей пар чисел.

Для успешной сдачи решения необходимо сделать предварительное объявление шаблонных функций перед всеми шаблонными функциями.

Пример предварительного объявления шаблонной функции

// Предварительное объявление шаблонных функций
template<typename T> T FuncA(T x);
template<typename T> void FuncB(T x);

// Объявляем шаблонные функции
template <typename T>
T FuncA(T x) { ... }

template <typename T>
void FuncB(T x) { ... }

Зачем это нужно и как это работает вы узнаете далее из наших курсов.
*/

// ========================= Предварительное объявление шаблонных функций =========================
template <typename T>
T Sqr(T value);

template <typename FirstType, typename SecondType>
std::pair<FirstType, SecondType> Sqr(const std::pair<FirstType, SecondType>& p);

template <typename FirstType, typename SecondType>
std::pair<FirstType, SecondType> operator*(const std::pair<FirstType, SecondType>& lhs, const std::pair<FirstType, SecondType>& rhs);

template <typename T>
std::vector<T> Sqr(const std::vector<T>& v_src);

template <typename KeyType, typename ValueType>
std::map<KeyType, ValueType> Sqr(const std::map<KeyType, ValueType>& map_src);
// ========================= КОНЕЦ предварительного объявления шаблонных функций =========================

// возвращает квадрат числа
template <typename T>
T Sqr(T value) {
    return value * value;
}

// возвращает квадрат пары (std::pair), по условиям задачи для "std::pair" в квадрат нужно возвести каждый элемент пары.
template <typename FirstType, typename SecondType>
std::pair<FirstType, SecondType> Sqr(const std::pair<FirstType, SecondType>& p) {
    return p * p;
}

// если вектор (или другой контейнер) содержит в качестве элементов не простые числа, а пары,
// нужно перегрузить оператор умножения "operator*" для std::pair
template <typename FirstType, typename SecondType>
std::pair<FirstType, SecondType> operator*(const std::pair<FirstType, SecondType>& lhs, const std::pair<FirstType, SecondType>& rhs) {
    return std::pair{lhs.first * rhs.first, lhs.second * rhs.second};
}

// возводит в квадрат std::vector<T>
template <typename T>
std::vector<T> Sqr(const std::vector<T>& v_src) {
    std::vector<T> v_dst;
    for (const auto& elm : v_src) {
        v_dst.push_back(Sqr(elm));
    }
    return v_dst;
}

// возводит в квадрат std::map<KeyType, ValueType>
template <typename KeyType, typename ValueType>
std::map<KeyType, ValueType> Sqr(const std::map<KeyType, ValueType>& map_src) {
    std::map<KeyType, ValueType> map_dst;
    for (const auto& [key, value] : map_src) {
        map_dst.insert({key, Sqr(value)});
    }
    return map_dst;
}

int main() {
    // std::cout << Sqr(2) << std::endl;

    // std::vector<int> v = {1, 2, 3};
    // std::cout << "vector:";
    // for (int x : Sqr(v)) {
    //     std::cout << ' ' << x;
    // }
    // std::cout << std::endl;

    std::map<int, std::pair<int, int>> map_of_pairs = {
        {4, {2, 2}},
        {7, {4, 3}}};
    std::cout << "map of pairs:" << std::endl;
    for (const auto& x : Sqr(map_of_pairs)) {
        std::cout << x.first << ' ' << x.second.first << ' ' << x.second.second << std::endl;
    }

    // auto v = Sqr(std::vector<int>{1, 2, 3, 4});
    // std::cout << "vector:";
    // for (int x : v) {
    //     std::cout << ' ' << x;
    // }
    // std::cout << std::endl;

    // тестируем std::map
    // std::map<std::pair<std::string, int>, int> m {
    //     {{"one", 1}, 101},
    //     {{"two", 2}, 102},
    //     {{"three", 3}, 103}
    // };

    // std::map<int, std::vector<double>> m{
    //     {1, {1.1, 1.11, 1.111}},
    //     {2, {2.2, 2.22, 2.222}},
    //     {3, {3.3, 3.33, 3.333}}};

    // auto res = Sqr(m);

    return 0;
}