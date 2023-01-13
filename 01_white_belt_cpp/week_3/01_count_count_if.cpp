#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    using namespace std::literals;

    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

template <typename Collection>
void PrintCollection(const Collection& collection) {
    using namespace std::literals;

    bool isFirst = true;
    for (const auto& elm : collection) {
        if (isFirst) {
            std::cout << elm;
            isFirst = false;
        } else {
            std::cout << " "s << elm;
        }
    }
    std::cout << std::endl;
}

bool IsOdd(int x) {
    return x % 2 != 0;
}

int main() {
    using namespace std::literals;

    // хотим посчитать, сколько раз встечается число "2" в нашем векторе
    std::vector<int> v = {1, 2, 3, 2, 5, 2, 4, 2};
    std::cout << "Исходный вектор: "s;
    PrintCollection(v);

    // функция std::count принимает диапазон, на котором ей нужно отработать
    int numToCount = 2;

    // вторым параметром std::count принимает элемент, количество вхождений которого нужно посчитать
    std::cout << "Количество вхождений в вектор элемента "s << numToCount << ": "s << std::count(v.begin(), v.end(), numToCount) << std::endl;

    // но на практике гораздо чаще возникают задачи, в которых нужно посчитать количество элементов, обладающих некоторым свойством
    // пусть необходимо посчитать количество НЕЧЕТНЫХ элементов в контейнере
    std::cout << "Количество нечетных элементов в контейнере: "s
              << std::count_if(v.begin(), v.end(),
                               [](int elm) {
                                   return elm % 2 != 0;
                               })
              << std::endl;

    int threshold = 3;
    // найти количество элементов, бОльших заданного значения ("threshold")
    std::cout << "Количество элементов в контейнере, бОльших "s << threshold << ": "s
              << std::count_if(v.begin(), v.end(),
                               [threshold](int elm) {
                                   return elm > threshold;
                               })
              << std::endl;

    // std::cout << "Количество нечетных элементов в контейнере: "s
    //           << std::count_if(v.begin(), v.end(), IsOdd)
    //           << std::endl;

    return 0;
}
