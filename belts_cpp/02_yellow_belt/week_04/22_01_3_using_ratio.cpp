#include <cstdint>  // to work with FIXED WIDTH integer types (since C++11)
#include <iostream>
#include <limits>
#include <ratio>
#include <string>

using namespace std::literals;

int main() {
    std::cout << "x = " << std::nano::num << "/" << std::nano::den << std::endl;

    // <ratio> defines a number of SI  (Système International) rational constants - type aliases for your convenience.
    // For example:
    // using micro = ratio<1, 1'000'000>;
    // using milli = ratio<1, 1'000>;

    return 0;
}
