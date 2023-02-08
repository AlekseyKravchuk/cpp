#include <chrono>
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>

using namespace std::literals::chrono_literals;

// написать constexpr-функцию для подсчета количества символов в строке
constexpr int myStrLen(const char* p) {
    return (*p) ? myStrLen(p+1) + 1 : 0;
}

int main() {
    constexpr int num = myStrLen("OK");
    std::cout << num << " characters." << std::endl;

    return 0;
}