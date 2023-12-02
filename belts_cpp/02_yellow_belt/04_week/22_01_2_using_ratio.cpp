#include <cstdint>  // to work with FIXED WIDTH integer types (since C++11)
#include <iostream>
#include <limits>
#include <ratio>
#include <string>

using namespace std::literals;

int main() {
    // Define a compile-time rational number, one_sixtieth = 1/60
    using one_sixtieth = std::ratio<1, 60>;
    std::cout << "one_sixtieth = " << one_sixtieth::num << "/" << one_sixtieth::den << std::endl;

    // Define a compile-time rational number, one_thirtieth = 1/30
    using one_thirtieth = std::ratio<1, 30>;
    std::cout << "one_thirtieth = " << one_thirtieth::num << "/" << one_thirtieth::den << std::endl;

    using result = std::ratio_add<one_sixtieth, one_thirtieth>::type;
    std::cout << "result = " << result::num << "/" << result::den << std::endl;
    return 0;
}
