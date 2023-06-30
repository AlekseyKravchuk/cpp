#include "test_runner.h"
#include "tests.h"

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    RUN_TEST(tr, TestAdvanced);

    return 0;
}