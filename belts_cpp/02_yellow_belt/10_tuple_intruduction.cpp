#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>  // std::pair
#include <vector>

using namespace std::literals;

int main() {
    // std::tuple<int, std::string, bool> t(7, "C++"s, true);
    std::tuple t1(7, "C++"s, true);  // благодаря автоматическому выводу параметров шаблонов в С++17

    // или так (до С++17), шаблонная функция "std::make_tuple" создает кортеж из самих значений)
    // auto t2 = std::make_tuple(7, "C++"s, true);

    // Здесь мы нарушаем семантику использования функции "std::tie"
    // "std::tie" используется для того, чтобы связать некоторые УЖЕ СУЩЕСТВУЮЩИЕ значения (lvalue), затем связать их в кортеж (tuple),
    // использовать некоторым образом (в первую очередь для того, чтобы сравнить) и забыть о связаных в кортеж значениях.
    // ===> !!! "std::tie" создает кортеж >>>>ИЗ ССЫЛОК<<<< !!! <===
    // auto t3 = std::tie(7, "C++"s, true);  //  error: cannot bind non-const lvalue reference of type ‘int&’ to an rvalue of type ‘int’

    // нужен способ обратиться к элементам кортежа
    // std::cout << std::get<1>(t2) << std::endl;
    std::cout << std::get<1>(t1) << std::endl;

    // пара - это частный случай кортежа, правда в этом случае намного удобнее обращаться к элементам пары
    // std::pair<int, std::string> p(7, "Python"s);
    std::pair p(7, "Python"s);
    std::cout << p.first << ", " << p.second << std::endl;

    std::map<int, std::string> digits{
        {1, "one"s},
        {2, "two"s},
        {3, "three"s},
        {4, "four"s}};

    bool isFirst = true;
    for (const auto& [digit, str] : digits) {
        if (isFirst) {
            std::cout << "{"s << digit << ": " << str << "}";
            isFirst = false;
        } else {
            std::cout << ", {"s << digit << ": " << str << "}";
        }
    }
    std::cout << std::endl;

    return 0;
}
