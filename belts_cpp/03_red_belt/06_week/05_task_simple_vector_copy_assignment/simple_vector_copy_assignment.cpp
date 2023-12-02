#include <algorithm>  // std::equal
#include <numeric>
#include <tuple>
#include <vector>

#include "simple_vector.h"
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

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCopyConstructor);
    RUN_TEST(tr, TestCopyAssignment);
}
