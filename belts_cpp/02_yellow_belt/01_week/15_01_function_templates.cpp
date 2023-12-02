#include <iostream>
#include <map>
#include <sstream>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

// решаем задачу возведения числа в квадрат
// в данной шаблонной функции нам необходимо, чтобы элемент типа T поддерживал функцию умножения на элемент типа T
template <typename T>
T Sqr(T x) {
    return x * x;
}

// пишем шаблонный оператор для пары
template <typename First, typename Second>
auto operator*(const std::pair<First, Second>& x, const std::pair<First, Second>& y) {
    return std::pair{x.first * y.first, x.second * y.second};
}

template <typename First, typename Second>
std::ostream& operator<<(std::ostream& os, const std::pair<First, Second>& p) {
    os << "pair{"s << p.first << ", "s << p.second << "}"s;
    return os;
}

int main() {
    // std::cout << Sqr(2) << std::endl;
    std::pair p{2.5, 3};

    // теперь хотим возводить в квадрат вещественные числа
    // std::cout << Sqr(2.5) << std::endl;
    std::cout << Sqr(p) << std::endl;
    return 0;
}
