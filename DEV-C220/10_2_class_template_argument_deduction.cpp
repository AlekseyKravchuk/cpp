#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "my_print.h"

using namespace std::literals;

template <typename T>
class A {
    T _a;  // внедренный объект типа Т

   public:
    // конструктор копирования необходим, чтобы проинициализировать внедренный объект
    A(const T& a) : _a(a) {
        std::cout << "Copy constructor was called."s << std::endl;
    }
};

int main() {
    A a1(std::vector(10, 20));

    std::cout << "OK"s << std::endl;

    return 0;
}
