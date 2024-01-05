#include <iostream>

using namespace std;

// общий шаблон
template <size_t N>
struct fib {
    enum { value = fib<N - 1>::value + fib<N - 2>::value };
};

// специализация шаблона для нуля
template <>
struct fib<0> {
    enum{ value = 1 };
};

// специализация шаблона для единицы
template <>
struct fib<1> {
    enum{ value = 1 };
};

int main() {
    int n = 10;
    cout << "fib(" << n << ") = " << fib<10>::value << endl;
    return 0;
}
