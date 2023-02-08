#include <chrono>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>

using namespace std::literals::chrono_literals;

// написать constexpr-функцию для сравнения 2-х строк
constexpr bool my_string_cmp(const char* s1, const char* s2) {
    return (*s1 == *s2 && *s1) ? my_string_cmp(s1 + 1, s2 + 1) : (*s1 - *s2);
}

int main() {
    constexpr const char* s1 = "abcz";
    constexpr const char* s2 = "abcd";
    std::cout << std::boolalpha << my_string_cmp(s1, s2) << std::endl;

    return 0;
}