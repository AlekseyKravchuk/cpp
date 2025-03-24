#include "hello.h"
#include <iostream>

std::ostream& hello(std::ostream& os) {
    os << "Hello, world!";
    return os;
}