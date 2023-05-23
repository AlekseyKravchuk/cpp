#include <iostream>
#include <set>
#include <vector>

#include "log_duration.h"

using namespace std::literals;

int main() {
    std::set<int> numbers;
    for (int i = 0; i < 3'000'000; ++i) {
        numbers.insert(i);
    }
    const int x = 1'000'000;

    {
        // глобальная функция "std::lower_bound" работает в 80 раз медленнее, чем одноименный метод у "std::set"
        LOG_DURATION("global lower_bound"s);
        std::cout << "x = "s << *std::lower_bound(numbers.begin(), numbers.end(), x) << ' ';
    }

    {
        LOG_DURATION("method std::set::lower_bound"s);
        std::cout << "x = "s << *numbers.lower_bound(x) << ' ';
    }

    return 0;
}
