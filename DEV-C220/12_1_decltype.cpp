#include <iostream>
#include <type_traits>

int f() {
    return 20;
}

int main() {
    {
        int i = 15;

        // Поскольку оператор присваивания возвращает ССЫЛКУ,
        // то тип переменной "n" будет ссылочного типа (int&),
        // а ссылка при своем определении должна быть ПРОИНИЦИАЛИЗИРОВАНА!!
        // => ERROR: ‘n’ declared as reference but not initialized
        // decltype(i = 4) n;

        // переменная "с" точно такого же типа, как возвращаемое функцией значение (т.е. "int")
        decltype(f()) c;  // "с" - типа "int"
        c = 155;
        std::cout << c << std::endl;
    }

    {
        int x = 1;
        double y = 5.5;
        decltype(x + y) z;  // "z" - типа "double"

        static_assert(std::is_same_v<double, decltype(z)>);
    }

    {
        // компилятор неявно приводит p0 к типу "const char*"
        // слева: const char8,
        // справа: константный массив строковых литералов, этот массив будет сопоставлен компилятором
        // ссылке "const char (&)[4]"
        auto p0 = "abc";
        static_assert(std::is_same_v<decltype(p0), const char*>);         // OK
        // static_assert(std::is_same_v<decltype(p0), decltype("abc")>);  // static assertion failed
        static_assert(std::is_same_v<decltype("bda"), decltype("abc")>);  // OK
    }

    return 0;
}
