#include <iostream>
#include <string>

using namespace std;

enum class Color {RED, BLUE};
enum class Fruit {BANANA, APPLE};

int main() {
    Color a = Color::RED;     // можем инициализировать переменную типа "enum" только с использованием разрешения области видимости
    Fruit b = Fruit::APPLE;  // можем инициализировать переменную типа "enum" только с использованием разрешения области видимости

    std::cout << static_cast<int>(a) << std::endl;
    std::cout << static_cast<int>(b) << std::endl;

    return 0;
}
