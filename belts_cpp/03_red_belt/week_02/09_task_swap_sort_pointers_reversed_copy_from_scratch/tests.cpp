#include "test_runner.h"
#include "functions.h"

void TestSwap() {
    int a = 1;
    int b = 2;
    Swap(&a, &b);
    ASSERT_EQUAL(a, 2);
    ASSERT_EQUAL(b, 1);

    std::string h = "world";
    std::string w = "hello";
    Swap(&h, &w);
    ASSERT_EQUAL(h, "hello");
    ASSERT_EQUAL(w, "world");
}

void TestSortPointers() {
    int one = 1;
    int two = 2;
    int three = 3;

    std::vector<int*> pointers;
    pointers.push_back(&two);
    pointers.push_back(&three);
    pointers.push_back(&one);

    SortPointers(pointers);

    ASSERT_EQUAL(pointers.size(), 3u);
    ASSERT_EQUAL(*pointers[0], 1);
    ASSERT_EQUAL(*pointers[1], 2);
    ASSERT_EQUAL(*pointers[2], 3);
}

void TestReverseCopyStepik() {
    {
        const size_t count = 7;

        int* source = new int[count];
        int* dest = new int[count];

        for (size_t i = 0; i < count; ++i) {
            source[i] = i + 1;
        }
        ReversedCopy(source, count, dest);
        const std::vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
        ASSERT_EQUAL(std::vector<int>(dest, dest + count), expected1);

        // Области памяти могут перекрываться
        ReversedCopy(source, count - 1, source + 1);
        const std::vector<int> expected2 = {1, 6, 5, 4, 3, 2, 1};
        ASSERT_EQUAL(std::vector<int>(source, source + count), expected2);

        delete[] dest;
        delete[] source;
    }
}

void TestReverseCopy_Non_Overlapping_src_dst() {
    {  // Вариант #1: области памяти НЕ перекрываются, при этом: source < dest (перемещаем вперед по пямяти)
        const size_t count = 7;

        int* dest = new int[count];
        int* source = new int[count];

        for (size_t i = 0; i < count; ++i) {
            source[i] = i + 1;
        }

        ReversedCopy(source, count, dest);
        const std::vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
        ASSERT_EQUAL(std::vector<int>(dest, dest + count), expected1);

        delete[] dest;
        delete[] source;
    }
}

void TestReverseCopy_Non_Overlapping_dst_src() {
    {  // Вариант #2: области памяти НЕ перекрываются, при этом: dest < source
        const size_t count = 7;

        int* source = new int[count];
        int* dest = new int[count];

        for (size_t i = 0; i < count; ++i) {
            source[i] = i + 1;
        }

        ReversedCopy(source, count, dest);
        const std::vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
        ASSERT_EQUAL(std::vector<int>(dest, dest + count), expected1);

        delete[] dest;
        delete[] source;
    }
}

void TestReverseCopy_Overlapping_src_dst() {
    {  // Вариант #3: области памяти перекрываются так, что: src > src
        const size_t count = 9;

        int* source = new int[count];

        for (size_t i = 0; i < count; ++i) {
            source[i] = i + 1;
        }

        ReversedCopy(source, 5, source + 2);
        const std::vector<int> target(source, source + count);
        const std::vector<int> expected = {1, 2, 5, 4, 3, 2, 1, 8, 9};
        ASSERT_EQUAL(target, expected);

        delete[] source;
    }
}

void TestReverseCopy_Overlapping_dst_src() {
    {  // Вариант #4: области памяти перекрываются так, что: dst < src
        const size_t count = 9;

        int* source = new int[count];

        for (size_t i = 0; i < count; ++i) {
            source[i] = i + 1;
        }

        // Области памяти могут перекрываться
        ReversedCopy(source + 2, 5, source);
        const std::vector<int> target(source, source + count);
        const std::vector<int> expected = {7, 6, 5, 4, 3, 6, 7, 8, 9};
        ASSERT_EQUAL(target, expected);

        delete[] source;
    }
}

void TestReverseCopy_Overlapping_dst_equals_src() {
    {
        // Вариант #5: области памяти перекрываются так, что: dst == src
        const size_t count = 9;

        int* source = new int[count];

        for (size_t i = 0; i < count; ++i) {
            source[i] = i + 1;
        }

        // Области памяти могут ПОЛНОСТЬЮ перекрываться
        ReversedCopy(source + 3, 5, source + 3);
        const std::vector<int> target(source, source + count);
        const std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        ASSERT_EQUAL(target, expected);

        delete[] source;
    }
}