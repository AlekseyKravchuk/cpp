#include <deque>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std::literals;

int main() {
    {
        LOG_DURATION("vector"s);
        std::vector<int> v;
        for (int i = 0; i < 100'000; ++i) {
            // вставляем элементы в начало вектора
            v.insert(std::begin(v), i);
        }
    }

    {
        LOG_DURATION("deque"s);
        std::deque<int> dq;
        for (int i = 0; i < 100'000; ++i) {
            // вставляем элементы в начало дека
            dq.insert(std::begin(dq), i);
        }
    }

    // Performance result:
    // vector: 278 milliseconds
    // deque: 2 milliseconds

    return 0;
}
