#include <iostream>

// #1
template <typename T, typename U>
void f(T x, U y) {
    std::cout << "1:"
              << "\tx = " << x << ", y = " << y << std::endl;
}

// #2
template <typename T>
void f(T x, T y) {
    std::cout << "2:"
              << "\tx = " << x << ", y = " << y << std::endl;
}

// #3
void f(int x, int y) {
    std::cout << "3:"
              << "\tx = " << x << ", y = " << y << std::endl;
}

/*
Принципы:
1. Точное соответствие лучше, чем приведение типа
2. Частное лучше, чем общее.
*/

int main() {
    // вызов функции от разных типов
    f(1, 10);    // 3
    f(1, 10.0);  // 1
    f('c', 'a'); // 2

    return 0;
}
