#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    int a = 11;
    int b = 3;

    std::cout << " integer division: a / b = "s << a / b << std::endl;
    std::cout << " double division: a / b = "s << a / static_cast<double>(b) << std::endl;
    
    return 0;
}
