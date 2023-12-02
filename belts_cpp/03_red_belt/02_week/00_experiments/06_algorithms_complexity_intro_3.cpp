#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include "log_duration.h"

using namespace std::literals;

const int NUMBER_COUNT = 1'000'000;
const int NUMBER = 7'654'321;
// const int NUMBER = 1;
const int QUERY_COUNT = 10'000;

int main() {
    std::vector<int> v;
    v.reserve(NUMBER_COUNT);

    for (int i = 0; i < NUMBER_COUNT; ++i) {
        v.push_back(i * 10);
    }

    {
        LOG_DURATION("lower_bound"s);
        for (int i = 0; i < QUERY_COUNT; ++i) {
            // auto it = std::lower_bound(std::begin(v), std::end(v), NUMBER);
            // if (it != v.end()) {
            //     std::cout << "Element is found, x = "s << *it << '\n';
            // }
            std::lower_bound(std::begin(v), std::end(v), NUMBER);
        }
    }

    {
        LOG_DURATION("std::find_if"s);
        for (int i = 0; i < QUERY_COUNT; ++i) {
            // auto it = std::find_if(std::begin(v), std::end(v),
            //                        [](int value) {
            //                            return value >= NUMBER;
            //                        });
            // if (it != v.end()) {
            //     std::cout << "Element is found, x = "s << *it << '\n';
            // }

            std::find_if(std::begin(v), std::end(v),
                                   [](int value) {
                                       return value >= NUMBER;
                                   });
        }
    }

    return 0;
}
