#include <iostream>

#include "string_set.h"
#include "test_runner.h"
#include "tests.h"

using namespace std::literals;

int main() {
    // {
    //     TestRunner tr;
    //     RUN_TEST(tr, TestSimple);
    //     RUN_TEST(tr, TestAnother);
    // }

    {
        StringSet strings;
        
        strings.Add("upper"s, 10);
        strings.Add("lower"s, 0);

        std::cout << strings.FindLast() << "\n"s;
        std::cout << strings.FindBest() << "\n"s;
    }

    return 0;
}