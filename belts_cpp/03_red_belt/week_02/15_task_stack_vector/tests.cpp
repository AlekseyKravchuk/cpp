#include "tests.h"

#include "stack_vector.h"
#include "test_runner.h"

void TestConstruction() {
    StackVector<int, 10> v;
    ASSERT_EQUAL(v.Size(), 0u);
    ASSERT_EQUAL(v.Capacity(), 10u);

    StackVector<int, 8> u(5);
    ASSERT_EQUAL(u.Size(), 5u);
    ASSERT_EQUAL(u.Capacity(), 8u);

    try {
        StackVector<int, 10> u(50);
        Assert(false, "Expect invalid_argument for too large size");
    } catch (std::invalid_argument&) {
    } catch (...) {
        Assert(false, "Unexpected exception for too large size");
    }
}

void TestPushBack() {
    StackVector<int, 5> v;
    for (size_t i = 0; i < v.Capacity(); ++i) {
        v.PushBack(i);
    }

    try {
        v.PushBack(0);
        Assert(false, "Expect overflow_error for PushBack in full vector");
    } catch (std::overflow_error&) {
    } catch (...) {
        Assert(false, "Unexpected exception for PushBack in full vector");
    }
}

void TestPopBack() {
    StackVector<int, 5> v;
    for (size_t i = 1; i <= v.Capacity(); ++i) {
        v.PushBack(i);
    }
    for (int i = v.Size(); i > 0; --i) {
        ASSERT_EQUAL(v.PopBack(), i);
    }

    try {
        v.PopBack();
        Assert(false, "Expect underflow_error for PopBack from empty vector");
    } catch (std::underflow_error&) {
    } catch (...) {
        Assert(false, "Unexpected exception for PopBack from empty vector");
    }
}