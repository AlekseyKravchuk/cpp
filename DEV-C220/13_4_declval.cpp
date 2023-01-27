#include <type_traits>  // std::is_same_v
#include <utility>      // std::declval
#include <vector>

// #include "my_print.h"

int main() {
    // проверяем, какой именно тип возвращает метод "std::vector::begin"
    std::vector<int> v{13, 4, 22};
    auto it = v.begin();

    // метод "std::vector::begin" возвращает итератор "std::vector<int>::iterator"
    static_assert(std::is_same_v<decltype(it), std::vector<int>::iterator>);

    return 0;
}
