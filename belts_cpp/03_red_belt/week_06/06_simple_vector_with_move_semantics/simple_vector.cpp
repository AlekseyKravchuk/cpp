#include "simple_vector.h"

#include <algorithm>  // std::equal
#include <numeric>
#include <tuple>
#include <vector>

#include "test_runner.h"

void TestCopyConstructor() {
    {
        SimpleVector<int> source_numbers = {};
        SimpleVector<int> target_numbers = source_numbers;
        ASSERT_EQUAL(source_numbers.Size(), target_numbers.Size());
        ASSERT_EQUAL(source_numbers.Capacity(), target_numbers.Capacity());
        ASSERT(std::equal(target_numbers.begin(), target_numbers.end(),
                          source_numbers.begin()));
    }

    {
        SimpleVector<int> source_numbers = {5, 6, 7, 8, 9};
        SimpleVector<int> target_numbers = source_numbers;
        ASSERT_EQUAL(source_numbers.Size(), target_numbers.Size());
        ASSERT_EQUAL(source_numbers.Capacity(), target_numbers.Capacity());
        ASSERT(std::equal(target_numbers.begin(), target_numbers.end(),
                          source_numbers.begin()));
    }
}

void TestCopyAssignment() {
    {
        SimpleVector<int> source_numbers;
        SimpleVector<int> target_numbers = {5, 6, 7, 8, 9};
        target_numbers = source_numbers;

        ASSERT_EQUAL(source_numbers.Size(), target_numbers.Size());
        ASSERT_EQUAL(source_numbers.Capacity(), target_numbers.Capacity());
        ASSERT(std::equal(target_numbers.begin(), target_numbers.end(),
                          source_numbers.begin()));
    }

    {
        SimpleVector<int> source_numbers = {5, 6, 7, 8, 9};
        SimpleVector<int> target_numbers = {1};
        target_numbers = source_numbers;

        ASSERT_EQUAL(source_numbers.Size(), target_numbers.Size());
        ASSERT_EQUAL(source_numbers.Capacity(), target_numbers.Capacity());
        ASSERT(std::equal(target_numbers.begin(), target_numbers.end(),
                          source_numbers.begin()));
    }

    {
        SimpleVector<int> source_numbers = {1};
        SimpleVector<int> target_numbers = {5, 6, 7, 8, 9};
        target_numbers = source_numbers;

        ASSERT_EQUAL(source_numbers.Size(), target_numbers.Size());
        ASSERT_EQUAL(source_numbers.Capacity(), target_numbers.Capacity());
        ASSERT(std::equal(target_numbers.begin(), target_numbers.end(),
                          source_numbers.begin()));
    }

    {
        SimpleVector<int> numbers(10);
        std::iota(numbers.begin(), numbers.end(), 1);

        SimpleVector<int> dest;
        ASSERT_EQUAL(dest.Size(), 0u);

        dest = numbers;
        ASSERT_EQUAL(dest.Size(), numbers.Size());
        ASSERT(dest.Capacity() >= dest.Size());
        ASSERT(std::equal(dest.begin(), dest.end(), numbers.begin()));
    }
}

void TestMoveConstructor() {
    {
        SimpleVector<int> source_numbers(1);
        SimpleVector<int> target_numbers = std::move(source_numbers);
        ASSERT_EQUAL(source_numbers.Size(), 0u);
        ASSERT_EQUAL(source_numbers.Capacity(), 0u);
    }
}

void TestMoveAssignmentOperator() {
    {
        SimpleVector<int> source_numbers = {1, 2, 3, 4, 5};
        SimpleVector<int> source_numbers_copy = source_numbers;
        SimpleVector<int> target_numbers = {10, 11, 12, 13};
        target_numbers = std::move(source_numbers);

        ASSERT_EQUAL(source_numbers.Size(), 0u);
        ASSERT_EQUAL(source_numbers.Capacity(), 0u);

        ASSERT_EQUAL(target_numbers.Size(), source_numbers_copy.Size());
        ASSERT_EQUAL(target_numbers.Capacity(), source_numbers_copy.Capacity());
        ASSERT(std::equal(source_numbers_copy.begin(), source_numbers_copy.end(), target_numbers.begin()));
    }

    {
        SimpleVector<int> source(15);
        SimpleVector<int> target(1);
        target = std::move(source);

        ASSERT_EQUAL(source.Size(), 0u);
        ASSERT_EQUAL(target.Size(), 15u);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCopyConstructor);
    RUN_TEST(tr, TestCopyAssignment);
    RUN_TEST(tr, TestMoveConstructor);
    RUN_TEST(tr, TestMoveAssignmentOperator);
}
