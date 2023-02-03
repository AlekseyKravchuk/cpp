#include <iostream>
#include <string>
#include <utility>  // std::exchange

using namespace std::literals;

int main() {
    // std::exchange(firstArg, secondArg) работает как setter:
    // T var = std::exchange(firstArg, secondArg), var = OLD value of firstArg
    //                                             firstArg = secondArg
    //                                             secondArg без изменений
    int x = 5;
    int y = 10;
    int z = 22;

    z = std::exchange(x, y);
    std::cout << "After std::exchange: " << std::endl;
    std::cout << "x = " << x << std::endl;  // x = 10: значение первого аргумента ("x") стало равно значению второго аргумента ("y")
    std::cout << "y = " << y << std::endl;  // y = 10: значение второго аргумента ("y") не изменилось
    std::cout << "z = " << z << std::endl;  // z = 5:  значение целевой переменной стало равно старому значению первого аргумента ("x")
    
    return 0;
}
