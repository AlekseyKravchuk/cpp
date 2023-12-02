#include <algorithm>
#include <deque>
#include <vector>

#include "log_duration.h"

using namespace std::literals;

int main() {
    const int SIZE = 10'000'000;

    std::vector<int> v;
    {
        LOG_DURATION("vector"s);
        for (int i = 0; i < SIZE; ++i) {
            v.push_back(i);
        }
    }

    std::deque<int> dq;
    {
        LOG_DURATION("deque"s);
        for (int i = 0; i < SIZE; ++i) {
            dq.push_back(i);
        }
    }

    // замеряем время на сортировку вектора и дека
    {
        LOG_DURATION("sort vector"s);
        std::sort(v.rbegin(), v.rend());
    }

    {
        LOG_DURATION("sort deque"s);
        std::sort(dq.rbegin(), dq.rend());
    }

    return 0;
}
