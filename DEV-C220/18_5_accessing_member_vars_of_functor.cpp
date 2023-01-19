#include <algorithm>  // std::copy_if
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>  // std::is_same, std::is_same_v
#include <utility>
#include <vector>

#include "my_print.h"

using namespace std::literals;

int main() {
    // Требуется увеличить на "delta" все элементы вектора
    // В результате выполнения операций "delta" на каждой итерации должна увеличиться на 10
    int delta = 10;

    std::vector<int> src{1, 2, 3, 4, 5, 10, 12};
    std::vector<int> dst;

    // благодаря захвату "delta" по ссылке
    // компилятор в своем анонимном функторе сохраняет адрес из внешнего контекста (адрес переменной "delta")
    // теперь мы можем модифицировать значение по данному адресу, тогда как сам адрес остается неизменным
    std::transform(src.begin(), src.end(),
                   std::back_inserter(dst),
                   [&delta](int val) {
                       delta += 10;
                       return val + 10;
                   });

    PrintCollection(dst, "Vector after applying lambda: ");
    std::cout << "delta = " << delta << std::endl;

    return 0;
}
