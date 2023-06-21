#include <algorithm>  // std::max, std::sort
#include <cstddef>    // ptrdiff_t
#include <string>
#include <utility>  // std::move
#include <vector>

#include "test_runner.h"

// обменивает местами значения, на которые указывают указатели first и second
template <typename T>
void Swap(T* first, T* second) {
    // T tmp{};
    // tmp = *first;
    // *first = *second;
    // *second = tmp;
    std::swap(*first, *second);
}

// сортирует указатели по значениям, на которые они указывают
template <typename T>
void SortPointers(std::vector<T*>& pointers) {
    std::sort(pointers.begin(), pointers.end(),
              [](T* lhs_ptr, T* rhs_ptr) {
                  return *lhs_ptr < *rhs_ptr;
              });
}

// копирует в обратном порядке count элементов, начиная с адреса в указателе src, в область памяти, начинающуюся по адресу dst.
// При этом:
// 1. каждый объект из диапазона [src; src + count) должен быть скопирован не более одного раза;
// 2. диапазоны [src; src + count) и [dst; dst + count) могут пересекаться
// 3. элементы в части диапазона [src; src + count), которая не пересекается с [dst; dst + count), должны остаться неизменными.
template <typename T>
void ReversedCopy(T* src, size_t count, T* dst) {
    T* src_end = src + count;
    T* dst_end = dst + count;

    T* max_of_starts = std::max(src, dst);
    T* min_of_ends = std::min(src_end, dst_end);

    // Если дипазоны НЕ перекрываются, используем стандартный алгоритм std::reverse_copy
    if (max_of_starts > min_of_ends) {
        std::reverse_copy(src, src_end, dst);
        return;
    }

    // Если дипазоны перекрываются, т.е. max_of_starts < min_of_ends (бОльшее из их начал меньше, чем меньший из их концов)
    T* ovelapped_begin = max_of_starts;
    T* ovelapped_end = min_of_ends;

    // изменяем порядок следования элементов в перекрывающейся части (overlapped part)
    std::reverse(ovelapped_begin, ovelapped_end);

    // в зависимости от расположения дипазонов [src, src_end) и [dst, dst_end) вызываем std::reverse_copy с нужными параметрами
    if (dst > src) {
        std::reverse_copy(src, dst, src_end);
    } else {
        std::reverse_copy(dst_end, src_end, dst);
    }
}

// // ========== Простое и элегантное решение, которое проходит грейдер ==========
// template <typename T>
// void ReversedCopy(T* source, size_t count, T* destination) {
//     std::vector<T> temp(source, source + count);
//     std::move(temp.rbegin(), temp.rend(), destination);
// }

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

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestSortPointers);

    RUN_TEST(tr, TestReverseCopyStepik);

    RUN_TEST(tr, TestReverseCopy_Non_Overlapping_src_dst);
    RUN_TEST(tr, TestReverseCopy_Non_Overlapping_dst_src);

    RUN_TEST(tr, TestReverseCopy_Overlapping_src_dst);
    RUN_TEST(tr, TestReverseCopy_Overlapping_dst_src);
    // RUN_TEST(tr, TestReverseCopy_Overlapping_dst_equals_src);

    return 0;
}
