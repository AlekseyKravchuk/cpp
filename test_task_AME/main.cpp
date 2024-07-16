#include <iostream>
#include <vector>
#include <algorithm>

#include "sorting_algorithms.h"
#include "utilities.h"
#include "tests.h"
#include "test_runner.h"

using namespace std;
using namespace std::literals;

int main() {
    // Оптимизация ввода-вывода для С++
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    TestAll();

    // Основная идея подхода: прежде чем делать программу многопоточной,
    // нужно выжать максимум из возможностей алгоритмов  исходя из особенностей входных данных (Data-Driven Approach)
    vector<int> v1 = GenerateRandomVectorOfInt();
    vector<int> v2(v1.begin(), v1.end());
    vector<int> v3(v1.begin(), v1.end());
    vector<int> v4(v1.begin(), v1.end());

    {
        LOG_DURATION("CountingSort"s);
        CountingSort(v1.begin(), v1.end());
    }

    {
        LOG_DURATION("CountingSort_MultiThread");
        CountingSort_MultiThread(v2.begin(), v2.end());
    }

    {
        LOG_DURATION("CountingSort_MultiThread_Merge");
        CountingSort_MultiThread_Merge(v3.begin(), v3.end());
    }

    {
        LOG_DURATION("std::sort"s);
        std::sort(v3.begin(), v3.end());
    }



    return 0;
}
