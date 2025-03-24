#include "hello.h"
#include <iostream>

int main() {
    std::stringstream ss;
    hello(ss);
    std::cout << ss.str() << std::endl;

}
