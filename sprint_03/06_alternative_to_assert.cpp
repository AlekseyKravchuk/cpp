#define NDEBUG  // Объявляем макрос NDEBUG
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

template <typename U, typename V>
void AssertEqual(const U& lhs, const V& rhs, const string hint) {
    if (lhs != rhs) {
        cout << "Assertion failed: "s << lhs << " != " << rhs << ". "s;

        if(!hint.empty()) {
            cout << "Hint: "s << hint;
        }

        cout << endl;

        abort();
    }
}

// int Add(int x, int y) {
//     return x * y;
// }

int Add(int a, int b) {
    // Ошибка здесь специально, чтобы вы увидели вывод информации об ошибке
    return a + b + 1;
}

int main() {
    // assert(Add(2, 3) == 5);
    // string hello = "hello"s;
    // AsserEqual(hello.length(), 5, "Add() must add its arguments"s);
    AssertEqual(Add(2, 3), 5, "Add() must add its arguments"s);

    return 0;
}