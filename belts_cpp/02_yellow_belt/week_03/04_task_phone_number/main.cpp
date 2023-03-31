#include "test_runner.h"
#include "tests.h"

void TestWorks() {
    AssertEqual(1, 1, "Just to check that Assert works");
}

int main() {
    // TestRunner tr;
    // tr.RunTest(TestWorks, "TestWorks");
    TestAll();

    return 0;
}