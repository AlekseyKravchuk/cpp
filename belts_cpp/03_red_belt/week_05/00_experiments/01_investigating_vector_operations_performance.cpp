#include <iostream>
#include <vector>

#include "../../log_duration.h"

int main() {
    int size = 1'000'000'000;

    {
        LOG_DURATION("push_back without preveious reserving");
        std::vector<int> v;
        for (int i = 0; i < size; ++i) {
            v.push_back(i);
        }
    }

    {
        LOG_DURATION("push_back WITH RESERVE");
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) {
            v.push_back(i);
        }
    }

    return 0;
}
