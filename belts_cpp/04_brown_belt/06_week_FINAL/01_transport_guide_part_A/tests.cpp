#include "tests.h"
#include "parse.h"
#include "test_runner.h"

using namespace std;

void TestTrimFunctions() {
    {
        string s = "                                                      ";
        string result_str = string{trim_view(s)};
        string expected_str;

        ASSERT_EQUAL(result_str.size(), 0u);
        ASSERT_EQUAL(result_str, expected_str);
    }

    {
        string s = " a ";
        string result_str = string{trim_view(s)};
        string expected_str = "a";

        ASSERT_EQUAL(result_str.size(), 1u);
        ASSERT_EQUAL(result_str, expected_str);
    }

    {
        string s =
                "       Tolstopaltsevo:  55.611087,   37.20829                     ";
        string expected_str = "Tolstopaltsevo:  55.611087,   37.20829";
        string result_str = string{trim_view(s)};
        ASSERT_EQUAL(result_str.size(), expected_str.size());
        ASSERT_EQUAL(expected_str, result_str);
    }


}

//void TestParsingFunctions() {
//
//}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestTrimFunctions);
}