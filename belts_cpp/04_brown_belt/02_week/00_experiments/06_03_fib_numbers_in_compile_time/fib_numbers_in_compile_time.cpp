#include <cstddef> // size_t
#include <iostream>

using namespace std;

// подсчет чисел Фиббоначи через рекурсию, но ЛИНЕЙНО (O(N))
template <size_t N>
struct fib {
    enum { value = fib<N-1>::value + fib<N-2>::value};
};

// терминальный элемент для 0 (специализация для 0)
template <>
struct fib<0> {
    enum {value = 1};
};

// терминальный элемент для 1 (специализация для 1)
template <>
struct fib<1> {
    enum {value = 1};
};

int main() {
    const int n = 5;
    cout << fib<n>::value << endl;

    return 0;
}


