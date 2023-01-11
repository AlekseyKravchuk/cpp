#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "my_print.h"

using namespace std::literals;

// TODO: понять, как работает конструкция "*out++ = std::move(*in);"
// Arthur O'Dwyer - Mastering the C++17 STL_ Make full use of the standard library components in C++17 (2017).pdf
// Глава 3. The Iterator-Pair Algorithms (Алгоритмы с парами итераторов), p. 68
// В русскоязычном варианте - стр. 74
template <class FwdIt, class T>
FwdIt my_remove(FwdIt first, FwdIt last, const T& value) {
    auto out = std::find(first, last, value);
    if (out != last) {
        auto in = out;
        while (++in != last) {
            if (*in == value) {
                // этот элемент нас не интересует
            } else {
                *out++ = std::move(*in);
            }
        }
    }
    return out;
}

int main() {
    std::vector<int> v{1, 2, 3, 2, 5, 2, 6, 2, 4, 8};
    PrintCollection(v, "Initial state of vector: ");

    // требуется удалить все четные элементы в векторе
    // v.erase(std::remove_if(v.begin(), v.end(), [](int val) { return val % 2 == 0; }),
    //         v.end());
    // PrintCollection(v, "Vector v after removing even numbers: "s);
    // std::cout << "SIZE of vector after std::vector::erase: "s << v.size() << std::endl;
    // std::cout << "CAPACITY of vector after std::vector::erase: "s << v.capacity() << std::endl;

    // уберем все двойки в векторе
    // "std::remove" возвращает "past-the-end iterator for the new range of values"
    int valToRemove = 2;
    // const auto new_end_it = std::remove(v.begin(), v.end(), valToRemove);
    const auto new_end_it = my_remove(v.begin(), v.end(), valToRemove);
    PrintCollection(v, "Vector v after applying std::remove: "s);
    std::cout << "new_end_it points to " << new_end_it - v.begin() << " position." << std::endl;
    std::cout << *new_end_it << std::endl;

    v.erase(new_end_it, v.end());
    PrintCollection(v, "Vector v after applying std::vector::erase: "s);

    return 0;
}
