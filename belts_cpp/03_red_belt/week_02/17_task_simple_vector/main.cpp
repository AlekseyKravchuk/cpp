#include <iostream>
#include <sstream>

#include "simple_vector.h"
#include "test_runner.h"
#include "tests.h"

using namespace std::literals;

template <class T>
std::ostream& operator<<(std::ostream& os, const SimpleVector<T>& v) {
    return os << "["s << Join(v, ", "s) << "]"s;
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConstruction);
    RUN_TEST(tr, TestPushBack);

    return 0;
}
