#include <iostream>

using namespace std::literals;

int main() {
    // int a = 5;
    int b = 8;
    int c = 12; 
    int d = 15;
    int e = 22;
    std::cout /* << a << ' ' */ << b << ' ' << c << ' ' << d << ' ' << e << std::endl;

    std::cout << *(&b + 1) << ' ' << *(&b + 2) << ' ' << *(&b + 3) << std::endl;
    return 0;
}
