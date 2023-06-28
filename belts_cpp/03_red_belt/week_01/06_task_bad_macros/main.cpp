#include <ostream>

#include "test_runner.h"

using namespace std::literals;

#define PRINT_VALUES(out, x, y) \
    out << (x) << std::endl     \
        << (y) << std::endl

int main() {
    TestRunner tr;
    tr.RunTest(
        []() {
            std::ostringstream output;
            PRINT_VALUES(output, 5, "red belt"s);
            // std::cout << output.str() << std::endl;
            ASSERT_EQUAL(output.str(), "5\nred belt\n"s);
        },
        "PRINT_VALUES usage example"s);
}