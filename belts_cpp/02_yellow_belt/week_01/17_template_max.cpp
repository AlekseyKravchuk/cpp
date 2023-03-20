#include <algorithm>
#include <iostream>

using namespace std::literals;

template <typename T>
auto Max(T a, T b) {
    return (a >= b) ? a : b;
}

int main() {
    // std::cout << Max(2, 3) << std::endl;

    // Теперь попробуем сравнить целое число и вещественное
    // В угловых скобках подсказываем компилятору, какой тип использовать вместо шаблонного параметра-типа "T"
    // std::cout << Max<double>(2, 3.5) << std::endl;

    // в стандартной библиотеке уже есть функция "std::max"
    std::cout << std::max<double>(2, 4.5) << std::endl;

    return 0;
}
