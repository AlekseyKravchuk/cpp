#include <iostream>
#include <type_traits>  // std::is_same_v
#include <vector>

using namespace std::literals;

// #include "my_print.h"

class A {
    int _t{};

   public:
    A() = default;
};

int main() {
    // {
    //     A a;
    //     const A& cr = a;

    //     auto a1 = cr;  // "a1" будет иметь тип "A" - "бантики" и "амперсанды" игнорируются

    //     // "cr2" будет иметь тип "const A&" - тип должен быть таким же как и выражение
    //     // инициализация ссылки обязательна
    //     decltype(cr) cr2 = cr;

    //     // "cr3" будет иметь тип "A"
    //     decltype(auto) cr3 = a;

    //     // "cr4" будет иметь тип "const A&"
    //     // конструкция "decltype(auto)" указывает компилятору: выведи тип ТОЧНО ТАКОЙ ЖЕ, как у инициализирующего выражения
    //     decltype(auto) cr4 = cr;
    // }

    {
        const std::vector<int> v(5);

        // оператор индексирования (subscript operator) для "std::vector<int>" будет возвращать
        // КОНСТАНТНУЮ ссылку - "const int&"
        // если используем "auto" без квалификаторов, то константность и ссылочность игнорируется
        auto a = v[0];
        static_assert(std::is_same_v<decltype(a), int>);  // OK

        // как только добавали "&" к "auto", константность и ссылочность больше не можем игнорировать
        auto& a_ref = v[0];
        static_assert(std::is_same_v<decltype(a_ref), const int&>);  // OK

        decltype(v[0]) b = 1;
        static_assert(std::is_same_v<decltype(b), const int&>);  // OK

        // ERROR: ‘c’ declared as reference but not initialized
        // decltype(v[0]) c;
    }

    {
        std::vector<int> v(5);
        // здесь будет ошибка, поскольку "d" будет иметь тип "int&" (неконстантная ссылка)
        // только КОНСТАНТНУЮ ссылку можно проинициализировать абсолютным значением.
        // decltype(v[1]) d = 1;
    }

    {
        auto a = 10;

        // оператор ПОСТФИКСНЫЙ инкремент возвращает ПО ЗНАЧЕНИЮ
        decltype(a++) b;

        static_assert(std::is_same_v<decltype(b), int>);

        std::cout << "a = "s << a << std::endl;
    }

    {
        auto a = 10;

        // оператор ПРЕФИКСНЫЙ инкремент возвращает ССЫЛКУ (в данном случае "int&"),
        // а ссылка ДОЛЖНА БЫТЬ ПРОИНИЦИАЛИЗИРОВАНА
        decltype(++a) b = a;

        static_assert(std::is_same_v<decltype(b), int&>);

        std::cout << "a = "s << a << std::endl;
    }

    {
        // есть ещё конструкция "decltype(auto)" - она позволяет установить тип для переменной
        // ТОЧНО ТАКОГО ЖЕ типа, как и инициализирующее выражение
        A a;
        const A& aRef = a;

        // просим компилятор вывести тип
        // результирующий выводимиый тип для "ref" должен быть ТОЧНО ТАКИМ ЖЕ,
        // как и инициализирующее выражение "aRef", т.е. "const A&"
        decltype(auto) ref = aRef;
        static_assert(std::is_same_v<decltype(ref), const A&>);
    }

    return 0;
}
