#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

/* 
Дано два целых числа A и B в диапазоне от 0 до 1 000 000 включительно.
Напишите программу, которая вычисляет целую часть частного от деления A на B.
Если B = 0, выведите "Impossible".
 */

int main() {
    int dividend{}, divisor{};
    std::cin >> dividend >> divisor;
    
    if(divisor) {
        std::cout << dividend / divisor;
    } else {
        std::cout << "Impossible";
    }

    return 0;
}
