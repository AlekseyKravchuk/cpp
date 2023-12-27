#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "simple_vector_2.h"
#include "test_runner.h"

void TestConstruction() {
    SimpleVector<int> empty;
    ASSERT_EQUAL(empty.Size(), 0u);
    ASSERT_EQUAL(empty.Capacity(), 0u);
    ASSERT(empty.begin() == empty.end());

    SimpleVector<std::string> five_strings(5);
    ASSERT_EQUAL(five_strings.Size(), 5u);
    ASSERT(five_strings.Size() <= five_strings.Capacity());

    for (auto& item : five_strings) {
        ASSERT(item.empty());
    }

    five_strings[2] = "Hello";
    ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack() {
    SimpleVector<int> v;

    for (int i = 10; i >= 1; --i) {
        v.PushBack(i);
        ASSERT(v.Size() <= v.Capacity());
    }

    std::sort(std::begin(v), std::end(v));

    const std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT(std::equal(std::begin(v), std::end(v), std::begin(expected)));
}

class StringNonCopyable : public std::string {
  public:
    using std::string::string;
    StringNonCopyable(std::string&& other) : std::string(std::move(other)) {}
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    SimpleVector<StringNonCopyable> strings;
    static const int SIZE = 10;

    for (int i = 0; i < SIZE; ++i) {
        strings.PushBack(StringNonCopyable(std::to_string(i)));
    }

    for (int i = 0; i < SIZE; ++i) {
        ASSERT_EQUAL(strings[i], std::to_string(i));
    }
}

void TestAssignmentToItself() {
    SimpleVector<std::string> strings;
    static const int SIZE = 10;

    for (int i = 0; i < SIZE; ++i) {
        strings.PushBack(std::to_string(i));
    }

    SimpleVector<std::string> deep_copy = strings;

    // self assignment
    strings = strings;

    ASSERT_EQUAL(strings, deep_copy);
}

void TestMovingAssignmentToItself() {
    SimpleVector<std::string> strings;
    static const int SIZE = 10;

    for (int i = 0; i < SIZE; ++i) {
        strings.PushBack(std::to_string(i));
    }

    SimpleVector<std::string> deep_copy = strings;

    // self assignment
    strings = std::move(strings);

    ASSERT_EQUAL(strings, deep_copy);
}

void TestChainedAssignment() {
    SimpleVector<std::string> strings;
    static const int SIZE = 10;

    for (int i = 0; i < SIZE; ++i) {
        strings.PushBack(std::to_string(i));
    }

    SimpleVector<std::string> deep_copy = strings;

    SimpleVector<std::string> a, b, c, d, e;
    a = b = c = d = e = strings;

    ASSERT_EQUAL(deep_copy, a);
    ASSERT_EQUAL(deep_copy, b);
    ASSERT_EQUAL(deep_copy, c);
    ASSERT_EQUAL(deep_copy, d);
    ASSERT_EQUAL(deep_copy, e);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConstruction);
    RUN_TEST(tr, TestPushBack);
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestAssignmentToItself);
    RUN_TEST(tr, TestMovingAssignmentToItself);
    RUN_TEST(tr, TestChainedAssignment);

    return 0;
}
