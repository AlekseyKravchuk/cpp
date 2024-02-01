#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>

#include "test_runner.h"

using namespace std;

/*
Рассмотрим, как КОНСТАНТНОСТЬ защищает нас от случайного изменения объектов в программе.
*/

// копирует из "src" в "dst" все элементы, которые НЕ равны параметру "value"
template <typename T>
void CopyIfNotEqual(const vector<T>& src, vector<T>& dst, T value) {
    // Здесь константность защищает нас от случайного изменения объектов, которые по своему смыслу не должны быть изменены.
    // Причем наша защита сработает на этапе компиляции.
      std::remove_copy(begin(src), end(src),
                       back_inserter(src),  // typo error - use "src" instead of "dst"
                       value);

    // // proper implementation:
    // std::remove_copy(begin(src), end(src),
    //                  back_inserter(dst),
    //                  value);
}

void TestCopyIfNotEqual() {
    vector<int> values = {1, 3, 8, 3, 2, 4, 8, 0, 9, 8, 6};
    vector<int> dest;
    CopyIfNotEqual(values, dest, 8);

    const vector<int> expected = {1, 3, 3, 2, 4, 0, 9, 6};

    ASSERT_EQUAL(dest, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCopyIfNotEqual);

    return 0;
}
