#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std::literals;

// Переписываем функцию суммирования элементов любого контейнера.
// Каждый контейнер библиотеки STL предоставляет псевдоним "value_type"
template <typename Container>
auto sum_1(const Container& container) {
    using type = typename Container::value_type;
    type sumCollector{};

    for (const type& val : container) {
        sumCollector += val;
    }

    return sumCollector;
}

// Улучшаем функцию.
// Выше мы внутри функции с помощью "value_type" сопоставляли псевдоним для целевого типа, который вернет функция.
// Теперь делаем то же самое, но на внешнем уровне.
template <typename Container>
using my_value_type = typename Container::value_type;

template <typename Container>
auto sum_2(const Container& container) {
    my_value_type<Container> sumCollector{};

    for (const auto& val : container) {
        sumCollector += val;
    }

    return sumCollector;
}

int main() {
    {
        std::vector<int> v_int{1, 12, 6, 11};
        std::cout << sum_1(v_int) << std::endl;

        std::vector<std::string> v_str{"1"s, "12"s, "6"s, "11"s};
        std::cout << sum_1(v_str) << std::endl;

        std::vector<double> v_double{1.2, 12.4, 6.1, 11.3};
        std::cout << sum_1(v_double) << std::endl;
    }

    std::cout << "================================="s << std::endl;
    {
        std::vector<int> v_int{1, 12, 6, 11};
        std::cout << sum_2(v_int) << std::endl;

        std::vector<std::string> v_str{"1"s, "12"s, "6"s, "11"s};
        std::cout << sum_2(v_str) << std::endl;

        std::vector<double> v_double{1.2, 12.4, 6.1, 11.3};
        std::cout << sum_2(v_double) << std::endl;
    }

    return 0;
}
