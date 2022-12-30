// Требуется написать шаблонную функцию, позволяющую просуммировать все элементы любого контейнера или массива

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

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

// нужно вернуть сумму, которая будет иметь такой же тип, как и тип элемента у "Container"
// пишем реализацию БЕЗ использования С++11
template <typename Container>
typename Container::value_type MySum(const Container& container) {
    // хотим заготовить переменную для результата
    // Для этого нужно: 1) знать тип, 2) как-то её проинициализировать некоторым дефолтным значением
    typename Container::value_type _sum = typename Container::value_type();

    // поскольку ссылка на контейнер константная, то и итераторы тоже должны быть const
    typename Container::const_iterator it = container.begin();
    typename Container::const_iterator endIt = container.end();

    // бежим по контейнеру и суммируем
    // для пользовательских типов должен быть реализован "operator+()"
    for (; it != endIt; ++it) {
        _sum = _sum + *it;
    }

    return _sum;
}

// нужно вернуть сумму, которая будет иметь такой же тип, как и тип элемента у "Container"
// пишем реализацию С использованием С++11
template <typename Container>
auto MySumImproved(const Container& container) {
    auto _sum = typename Container::value_type();

    // поскольку ссылка на контейнер константная, то и итераторы тоже должны быть const
    auto it = container.begin();
    auto endIt = container.end();

    // бежим по контейнеру и суммируем
    // для пользовательских типов должен быть реализован "operator+()"
    for (; it != endIt; ++it) {
        _sum = _sum + *it;
    }

    return _sum;
}


int main() {
    std::vector<int> v(10, 1);
    // PrintCollection(v);
    auto result_1 = MySum(v);
    std::cout << result_1 << std::endl;
    std::cout << "***********************"s << std::endl;

    std::list<std::string> lst(10, "A"s);
    // PrintCollection(lst);
    auto result_2 = MySum(lst);
    std::cout << result_2 << std::endl;
    std::cout << "***********************"s << std::endl;

    return 0;
}
