#include <iostream>

#include "rectangle.h"

using namespace std::literals;

int main() {
    Rectangle rectangle(10, 5);
    std::cout << "Perimeter: " << rectangle.GetPerimeter() << std::endl;
    std::cout << "Area: " << rectangle.GetArea() << std::endl;
    return 0;
}
