#include <functional>  // std::bind
#include <iomanip>
#include <iostream>

using namespace std;

void foo(int a, string str, double d) {
    cout << "a = " << a << '\t'
         << R"(str = ")" << str << R"(")" << '\t'
         << "d = " << d << endl;
}

int main() {
    foo(5, "some words", 2.55);

    auto f_1 = std::bind(foo, 1, "2", 3.9);
    f_1();

    // в качестве placeholders мы указываем те аргументы, которые НЕ хотим фиксировать
    // 2-ой аргумент НЕ фиксирован => должен быть представлен при вызове
    auto f_2 = std::bind(foo, 1, std::placeholders::_1, 3.9);
    f_2("tricky");

    // 1-ый аргумент НЕ фиксирован => должен быть представлен при вызове
    auto f_3 = std::bind(foo, std::placeholders::_1, "stub", 3.9);
    f_3(5);

    // 1-ый и 2-ой аргументы НЕ фиксированы => должны быть представлены при вызове
    auto f_4 = std::bind(foo, std::placeholders::_1, std::placeholders::_2, 3.9);
    f_4(111, "non-default string");

    // 1-ый и 3-ий аргументы НЕ фиксированы => должны быть представлены при вызове
    auto f_5 = std::bind(foo, std::placeholders::_1, "3.9", std::placeholders::_2);
    f_5(111, 9.999);

    // из исходной функции с помощью std::bind можно сделать другую "функцию" путем перестановки аргументов
    auto func = [](int x, int y) -> void {
        cout << "x = " << x << '\t'
             << "y = " << y << '\n';
    };

    cout << "===================================================" << endl;

    auto func_reverse_args = std::bind(func, std::placeholders::_2, std::placeholders::_1);
    int x = 10;
    int y = 555;
    func(x, y);
    func_reverse_args(x, y);

    return 0;
}
