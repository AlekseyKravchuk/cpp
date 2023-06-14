#include <algorithm>
#include <string>
#include <vector>

#include "test_runner.h"

// обменивает местами значения, на которые указывают указатели first и second
template <typename T>
void Swap(T* first, T* second) {
    T* tmp = nullptr;
    tmp = first;
    first = second;
    second = tmp;
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
    T* last = src + count;

    for ( ; src != last; ++dst) {
        *dst = *(--last);
    }
}

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

void TestReverseCopy() {
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

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestSortPointers);
    RUN_TEST(tr, TestReverseCopy);
    return 0;
}
