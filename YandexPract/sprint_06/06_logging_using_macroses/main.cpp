#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "log_duration.h"

using Clock = std::chrono::steady_clock;
using namespace std::literals;

std::vector<int> ReverseVector(const std::vector<int>& sourceVector) {
    std::vector<int> res;
    for (int i : sourceVector) {
        res.insert(res.begin(), i);
    }

    return res;
}

int CountPops(const std::vector<int>& sourceVector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (sourceVector[i]) {
            ++res;
        }
    }

    return res;
}

void AppendRandom(std::vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        v.push_back(rand() % 2);
    }
}

void Operate() {
    {
        // LogDuration UNIQUE_VAR_NAME_PROFILE("Total"s);
        LOG_DURATION("Total"s);
        std::vector<int> random_bits;
        std::vector<int> reversed_bits;

        // "1 << 17" is equivalent to "2^17" = 131072
        static const int N = 1 << 17;

        {
            // LogDuration UNIQUE_VAR_NAME_PROFILE("Append random"s);
            LOG_DURATION("Append random"s);
            // заполним вектор случайными числами 0 и 1
            AppendRandom(random_bits, N);
        }

        {
            // LogDuration UNIQUE_VAR_NAME_PROFILE("Reverse"s);
            LOG_DURATION("Reverse"s);
            // перевернём вектор задом наперёд
            reversed_bits = ReverseVector(random_bits);
        }

        {
            // LogDuration UNIQUE_VAR_NAME_PROFILE("Counting"s);
            LOG_DURATION("Counting"s);
            // посчитаем процент единиц на начальных отрезках вектора
            for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
                double rate = CountPops(reversed_bits, 0, i) * 100. / i;
                std::cout << "After "s << i << " bits we found "s << rate << "% pops"s << std::endl;
            }
        }
    }
}

int main() {
    Operate();
}