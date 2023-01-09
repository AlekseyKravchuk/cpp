// move-семантика и захват выражений

#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    std::string s{"abc"s};

    [str = std::move(s)](){
        std::cout << "str (from lambda) = "s << str << std::endl;
    }();

    std::cout << "str (from lambda) = "s << s << std::endl;

    [&str = s](){
        str = "123"s;
    }();

    // по адресу, который компилятор сформировал в анонимном функторе (с именем "str")
    // и проинициализировал значением, лежащем по адресу из внешнего контекста (адресом переменной "s")
    std::cout << "s (from extern context) = "s << s << std::endl;

    // в данном случае компилятор в анонимном функторе создаст переменную-член с именем "str"
    // и проинициализирует её посредством вызова конструктора для "std::string"
    [str{"Hello, world!"s}](){std::cout << str << std::endl;}();

    return 0;
}
