#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; i += 15) {
        int number = rand();
        v.reserve(n);

        // мы можем заполнить 15 элементов вектора,
        // но не более, чем нам осталось до конца:
        int count = min(15, n - i);

        for (int j = 0; j < count; ++j)
            // операцию сдвига битов вы уже видели в этой программе
            // на этот раз двигаем вправо, чтобы нужный бит оказался самым последним
            v.push_back((number >> j) % 2);
    }
}

void Operate() {
    LOG_DURATION("Total"s);
    vector<int> random_bits;
    static const int N = 1 << 17;

    // заполним вектор случайными числами 0 и 1
    {
        LOG_DURATION("Append random"s);
        AppendRandom(random_bits, N);
    }

    // reverse vector
    vector<int> reversed_bits;
    {
        LOG_DURATION("Reverse"s);

        reversed_bits = ReverseVector(random_bits);
    }

    {
        LOG_DURATION("Counting"s);
        // посчитаем процент единиц на начальных отрезках вектора
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " digits we found "s << rate << "% pops"s << endl;
        }
    }
}

int main() {
    Operate();
    return 0;
}
