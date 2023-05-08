#include <algorithm>  // std::partition
#include <numeric>    // std::accumulate

#include "00_print.h"

int main() {
    std::vector<double> temperatures{65, 75, 56, 48, 31, 28, 32, 29, 40, 41, 44, 50};
    std::cout << temperatures << std::endl;

    // Compute the average value
    auto average = std::accumulate(temperatures.begin(), temperatures.end(), 0.0) / static_cast<double>(temperatures.size());

    std::cout << "Average temperature: " << average << std::endl;

    // std::partition переставляет элементы в контейнере т.о., что в 1-ой части располагаются элементы, для которых предикат возвращает "true"
    // возвращает итератор на первый элемент 2-ой части (диапазона), для которого предикат возвращает "false"
    // "минусы"  алгоритма: он не гарантирует относительный порядок следования элементов (в изначальном состоянии контейнера)
    // auto it = std::partition(temperatures.begin(), temperatures.end(),
    //                          [average](double t) {
    //                              return t > average;
    //                          });

    // // Чтобы сохранить относительный порядок следования элементов в контейнере, используется алгоритм std::stable_partition
    auto it = std::stable_partition(temperatures.begin(), temperatures.end(),
                             [average](double t) {
                                 return t > average;
                             });

    std::cout << "=============================="s << std::endl;
    std::cout << temperatures << std::endl;
    std::cout << "Iterator pointing to the beginning of 2-nd group points to: " << *it
              << " ("s << it - temperatures.begin() << " index)"s << std::endl;

    return 0;
}
