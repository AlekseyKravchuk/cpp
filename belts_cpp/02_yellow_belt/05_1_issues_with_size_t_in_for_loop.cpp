#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    // Пусть стоит задача вывести все элементы вектора, кроме последнего
    // std::vector<int> v = {4, 5, -8, 2};
    std::vector<int> v = {4, 8};
    // std::vector<int> v;

    // вроде бы всё отлично, код работат
    // но что, если вектор оказался пустым?
    // если вектор пустой, программа падает!!!!!
    // for (size_t i = 0; i < v.size() - 1; ++i) {
    //     std::cout << "v["s << i << "]: "s << v[i] << std::endl;
    // }

    // ПРИНЦИП: НИКОГДА НЕ ВЫЧИТАТЬ ИЗ БЕЗЗНАКОВЫХ ТИПОВ
    // Выход из ситуации: можно перенести знак '-' в левую часть выражения от знака сравнения "<"
    // т.е. заменяем вычитание сложением
    for (size_t i = 0; i + 1 < v.size(); ++i) {
        std::cout << "v["s << i << "]: "s << v[i] << std::endl;
    }

    return 0;
}
