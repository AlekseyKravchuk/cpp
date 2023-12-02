#include <iostream>
#include <map>
#include <sstream>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

// решаем задачу возведения числа в квадрат
// данная шаблонная функция принимает некий абстрактный тип "T",
// при этом необходимо, чтобы тип "T" поддерживал умножение элементов типа "T" друг на друга
template <typename T>
T Sqr(T value) {
    return value * value;
}

// перегружаем шаблонный оператор умножения для пары любого типа
template <typename FirstType, typename SecondType>
std::pair<FirstType, SecondType> operator*(const std::pair<FirstType, SecondType>& lhs, const std::pair<FirstType, SecondType>& rhs) {
    return std::pair{lhs.first * rhs.first, lhs.second * rhs.second};
}

template <typename FirstType, typename SecondType>
std::ostream& operator<<(std::ostream& os, const std::pair<FirstType, SecondType>& p) {
    os << "("s << p.first << ", "s << p.second << ")"s;
    return os;
}

int main() {
    std::cout << Sqr(2) << std::endl;

    // теперь хотим возводить в квадрат вещественные числа
    std::cout << Sqr(2.5) << std::endl;

    // теперь хотим иметь возможность возводить в квадрат пары (std::pair)
    // error: no match for ‘operator*’ (operand types are ‘std::pair<double, int>’ and ‘std::pair<double, int>’)
    // => требуется перегрузить оператор умножения для типа std::pair<FirstType, SecondType>
    std::pair p{2.5, 3};
    std::cout << Sqr(p) << std::endl;
    return 0;
}
