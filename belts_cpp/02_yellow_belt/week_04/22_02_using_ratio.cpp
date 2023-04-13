#include <chrono>
#include <cstdint>  // to work with FIXED WIDTH integer types (since C++11)
#include <iostream>
#include <limits>
#include <ratio>
#include <string>

using namespace std::literals;

int main() {
    // Следующие 2 строчки кода демонстрируют ЯВНОЕ (explicit) преобразование между различными единицами времени (different units of time).

    // первый объект "duration", "d7", представляет продолжительность, равную 30 секундам
    std::chrono::duration<long> d7{30};  // seconds

    // Второй объект "duration", "d8", представляет продолжительность, равную 0.5 минут.
    // Это достигается посредством вызова соответствующего конвертирующего конструктора, который принимает другое значение "duration".
    // Преобразование в этом направлении может дать в результате НЕЦЕЛОЧИСЛЕННОЕ значение,
    // => в качестве ПЕРВОГО шаблонного параметра класса "std::chrono::duration" мы ДОЛЖНЫ использовать тип с плавающей точкой (floating-point type)
    std::chrono::duration<double, std::ratio<60>> d8{d7};  // minutes

    // если в качестве ПЕРВОГО шаблонного параметра (class Rep) использовать целочисленный тип, то получим ошибку компиляции
    // std::chrono::duration<long, std::ratio<60>> d9{d7};  // ERROR

    // Однако можно использовать "duration_cast", но при этом получим продолжительность в ноль минут:
    auto d9{std::chrono::duration_cast<std::chrono::duration<long, std::ratio<60>>>(d7)};


    return 0;
}
