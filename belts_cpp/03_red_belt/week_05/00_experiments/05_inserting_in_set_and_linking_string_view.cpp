#include <iostream>
#include <set>
#include <string>
#include <string_view>

using namespace std::literals;

int main() {
    std::string s1 = "ABC"s;
    std::string_view sw1 = s1;

    std::string s2 = "ABC"s;
    std::string_view sw2 = s2;

    std::cout << (sw1 == sw2)  << std::endl;

    return 0;
}
