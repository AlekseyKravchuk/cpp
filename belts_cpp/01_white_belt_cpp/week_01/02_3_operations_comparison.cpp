#include <iostream>
#include <string>

using namespace std::literals;

int main() {
    std::string a = "firez"s;
    std::string b = "firewall"s;

    if (a < b) {
        std::cout << "a is less than b"s << std::endl;
    }

    std::cout << static_cast<unsigned>('0') << std::endl;
    std::cout << static_cast<unsigned>('z' - '0') << std::endl;
    std::cout << static_cast<unsigned>('w' - '0') << std::endl;
    
    return 0;
}
