#include <cstddef>  // std::size_t, fixed width integer types aka int8_t, int16_t, ...
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

using namespace std::literals;

int main() {
    // std::cout << "max value for long: "s << std::numeric_limits<long int>::max() << std::endl;
    // std::cout << "min value for long: "s << std::numeric_limits<long int>::min() << std::endl;

    // std::cout << "max value for int64_t: "s << std::numeric_limits<int64_t>::max() << std::endl;
    // std::cout << "min value for int64_t: "s << std::numeric_limits<int64_t>::min() << std::endl;

    // std::cout << "max value for uint32_t: "s << std::numeric_limits<uint32_t>::max() << std::endl;
    // std::cout << "min value for uint32_t: "s << std::numeric_limits<uint32_t>::min() << std::endl;

    std::cout << "max value for uint16_t: "s << std::numeric_limits<uint16_t>::max() << std::endl;
    std::cout << "min value for uint16_t: "s << std::numeric_limits<uint16_t>::min() << std::endl;

    return 0;
}
