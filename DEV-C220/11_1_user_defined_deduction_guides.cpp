#include <string>

using namespace std::literals;

template <typename T>
class A {
    T _a;

   public:
    A(const T& a) : _a(a) {}
};

// без ниже лежащей строки (User-Defined Deduction Guide) будет ошибка: array used as initializer
// (пользовательские подсказки для выведения типа компилятором)
// такие подсказки пишутся вне класса (там же, где и объявления класса)
// если в качестве параметра конструктора появляется конструкция "const char*",
// то, уважаемый компилятор, выводи тип параметра шаблона "T", как "std::string"
A(const char*) -> A<std::string>;  // подсказка компилятору для выведения типа


int main() {
    A a1("QWERTY"s);

    return 0;
}
