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
    // Иногда без "trailing return type" в лямбде не обойтись
    int x = 15;

    // в этом случае перегруженный "operator(auto& T)" вернет "по значению"
    auto lambda1 = [](auto& x) { return x; };

    // => все модификации, применяемые к переменной "x_expected_as_ref" не окажут влияния на переменную "x"
    auto x_expected_as_ref = lambda1(x);

    // Проверяем возвращаемый лямбдой тип.
    static_assert(std::is_same_v<decltype(x_expected_as_ref), int>);

    x_expected_as_ref += 100;  // это действие не повлияет на переменную "x"

    std::cout << "x_expected_as_ref: "s << x_expected_as_ref << std::endl;
    std::cout << "x: "s << x << std::endl;

    std::cout << std::endl;

    // Следует иметь в виду, что auto никогда не выводится как ссылочный тип,
    // поэтому по умолчанию лямбда возвращает нессылочный тип.
    // => указываем возвращаемый лямбдой тип ЯВНО
    auto lambda2 = [](auto& x) -> auto& { return x; };
    auto& x_ref = lambda2(x);
    x_ref += 200;
    static_assert(std::is_same_v<decltype(x_ref), int&>);

    std::cout << "x_ref: "s << x_ref << std::endl;
    std::cout << "x: "s << x << std::endl;

    // auto lambda3 = [](auto& x) -> auto& { return x; };
    // static_assert(std::is_same_v<decltype(lambda3(x)), int&>);

    return 0;
}
