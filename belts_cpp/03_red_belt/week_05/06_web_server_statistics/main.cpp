#include "http_request.h"
#include "stats.h"
#include "test_runner.h"
#include "tests.h"

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestParsing);
    RUN_TEST(tr, TestBasic);
    RUN_TEST(tr, TestAbsentParts);
}