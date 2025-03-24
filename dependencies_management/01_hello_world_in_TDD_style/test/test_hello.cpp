#include "hello.h"
#include <cassert>

void test_hello() {
    std::stringstream ss;
    hello(ss);
//    assert("Hello, world" == ss.str());
    assert("Hello, world!" == ss.str());
    std::cout << R"(Test 'test_hello()' succeeded.)" << std::endl;
}

void run_tests() {
    test_hello();
}

int main() {
    run_tests();
}