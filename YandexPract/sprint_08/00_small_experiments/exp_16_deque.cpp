#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

// Пусть нам нужно вставить набор чисел, но при этом мы заранее не знаем, сколько у нас будет этих самых чисел
// Т.е. мы не можем вызвать метод std::vector::reserve
int main() {
    // const int SIZE = 100'000'000;
    int SIZE;
    cin >> SIZE;
    vector<int> v;
    deque<int> q;

    {
        LOG_DURATION("vector"s);
        for (int i = 0; i < SIZE; ++i) {
            v.push_back(i);
        }
    }

    {
        LOG_DURATION("deque"s);
        for (int i = 0; i < SIZE; ++i) {
            q.push_back(i);
        }
    }

    return 0;
}
