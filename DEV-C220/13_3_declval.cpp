#include <type_traits>  // std::is_same_v
#include <utility>      // std::declval
#include <vector>

// #include "my_print.h"

// DECLVAL - шаблон функции std::declval(), которая преобразует любой тип T к "rvalue-reference" (T&&),
// что позволяет использовать методы класса в "decltype" без использования конструкторов

// в класса "А" default-конструктор имеет спецификатор доступа "private"
class A {
   private:
    A() {}
    public:
    int AFunc() const {
        return 1;
    }
};

int main() {
    // нужно добраться до типа, который возвращает метод "AFunc"
    // error: ‘A::A()’ is private within this context
    // decltype(A().AFunc()) asAFuncReturnType = 20;

    decltype(std::declval<A>().AFunc()) asAFuncReturnType = 20;
    static_assert(std::is_same_v<decltype(asAFuncReturnType), int>);

    return 0;
}
