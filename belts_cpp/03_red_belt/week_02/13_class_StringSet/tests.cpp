#include "tests.h"
#include "string_set.h"

void TestSimple() {
    StringSet strings;
    strings.Add("upper", 10);
    strings.Add("lower", 0);
    ASSERT_EQUAL(strings.FindLast(), "lower");
    ASSERT_EQUAL(strings.FindBest(), "upper");
}

void TestAnother() {
    StringSet strings;

    strings.Add("apple", 1);
    strings.Add("orange", 3);
    ASSERT_EQUAL(strings.FindLast(), "orange");
    ASSERT_EQUAL(strings.FindBest(), "orange");

    strings.Add("plum", -5);
    ASSERT_EQUAL(strings.FindLast(), "plum");
    ASSERT_EQUAL(strings.FindBest(), "orange");

    strings.Add("lemon", 5);
    ASSERT_EQUAL(strings.FindLast(), "lemon");
    ASSERT_EQUAL(strings.FindBest(), "lemon");
}