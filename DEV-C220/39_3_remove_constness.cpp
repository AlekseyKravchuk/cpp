#include <iostream>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std::literals;

class A {
   public:
    A(int a) : _a(a) {}
    A() : _a(200) {}

    int Get() const {
        return _a;
    }

   private:
    int _a{};
};

std::ostream& operator<<(std::ostream& os, const A& objRef) {
    os << objRef.Get();
    return os;
}

int main() {
    const std::vector<A> v = {A{15}, A{20}, A{35}};

    // либо выписываем полностью
    // std::remove_reference<decltype(*v.begin())>::type x;
    std::remove_reference_t<decltype(*v.begin())> x;

    // либо используем шаблон шаблона
    static_assert(std::is_same_v<decltype(x), const A>);

    // также можно убрать "константность"
    std::remove_const_t<decltype(x)> y;
    static_assert(std::is_same_v<decltype(y), A>);

    // нужно помнить, что снимается константность верхнего уровня (top-level)
    std::remove_const<const char>::type a;
    static_assert(std::is_same_v<decltype(a), char>);

    // тип "const char*" - это НЕконстантный указатель на константное значение
    std::remove_const<const char*>::type b;
    static_assert(std::is_same_v<decltype(b), const char*>);  // по-прежнему имеем "const char*"

    // тип "char* const" - это константный указатель на НЕконстантное значение
    std::remove_const<char* const>::type c;
    static_assert(std::is_same_v<decltype(c), char*>);  // теперь имеем "char*", константность указателя снята

    return 0;
}
