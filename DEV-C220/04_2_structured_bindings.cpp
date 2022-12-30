#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <string>
#include <vector>

using namespace std;


int main() {


    double d = 1.1;
    int i = 15;
    std::pair<const int, double&> p(i, d);

    // Если пара, структура, кортеж содержит ссылки или константы,
    // то ПРИ ДЕКОМПОЗИЦИИ соответствующие имена тоже станут ссылками (b ссылка на второй элементы пары)
    // и константами (а - это "const int")
    auto [a, b] = p;
    b += 10.5;
    std::cout << a << ", "s << b << std::endl;
    std::cout << i << ", "s << d << std::endl;
    std::cout << p.first << ", "s << p.second << std::endl;

    return 0;
}
