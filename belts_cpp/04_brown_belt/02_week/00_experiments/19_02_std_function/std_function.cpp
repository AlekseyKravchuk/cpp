#include <iostream>
#include <functional>  // std::function

using namespace std;

// int subtract(int x, int y) {
//     return x - y;
// }

// int sum(int x, int y) {
//     return x + y;
// }

template <typename T>
T sum(T x, T y) {
    return x + y;
}

template <typename T>
T subtract(T x, T y) {
    return x - y;
}

/*
При использовании объектов-функций и лямбда-выражений следует помнить, что они не эквивалентны указателям на функции.
Чтобы упростить совместное использование указателей на функции, объектов-функций и лямбда-выражений модуль <functional>
определяет шаблон std::function<>.
std::function представляет нечто, что может вызываться как функция - это может быть и указатель на функцию, и объект-функция,
и лямбда-выражение.
*/

// template <typename T>
// T operation(std::function<T(T, T)> func, T a, T b) {
//     return func(a, b);
// }

int operation(std::function<int(int, int)> func, int a, int b) {
    return func(a, b);
}

int main() {
    cout << "operation(sum, 9, 10) = "      << operation(sum<int>, 9, 10)      << endl;
    cout << "operation(subtract, 9, 10) = " << operation(subtract<int>, 9, 10) << endl;

    // // works fine
    // cout << "operation(sum, 9, 10) = " << operation([](int x, int y){return x + y;}, 9, 10) << endl;
    // cout << "operation(subtract, 9, 10) = " << operation([](int x, int y){return x - y;}, 9, 10) << endl;

    return 0;
}
