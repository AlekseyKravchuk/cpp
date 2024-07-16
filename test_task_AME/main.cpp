#include <algorithm>
#include <iostream>
#include <vector>

#include "sorting_algorithms.h"
#include "test_runner.h"
#include "tests.h"
#include "utilities.h"

using namespace std;
using namespace std::literals;

int main(int argc, char* argv[]) {
    int sample_size = 0;

    if (argc == 2) {
        istringstream iss(argv[1]);
        if (!(iss >> sample_size)) {
            std::cerr << "Invalid number: " << argv[1] << '\n';
        } else if (!iss.eof()) {
            std::cerr << "Trailing characters after number: " << argv[1] << '\n';
        }
    } else {
        sample_size = 1'000'000;
    }

    // Оптимизация ввода-вывода для С++
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    TestAll();

    vector<int> v1 = GenerateRandomVectorOfInt(sample_size);
    vector<int> v2(v1.begin(), v1.end());
    vector<int> v3(v1.begin(), v1.end());
    vector<int> v4(v1.begin(), v1.end());

    {
        LOG_DURATION("CountingSort"s);
        CountingSort(v1.begin(), v1.end());
    }

    {
        LOG_DURATION("CountingSort_MultiThread"s);
        CountingSort_MultiThread(v2.begin(), v2.end());
    }

    {
        LOG_DURATION("CountingSort_MultiThread_Merge"s);
        CountingSort_MultiThread_Merge(v3.begin(), v3.end());
    }

    {
        LOG_DURATION("std::sort"s);
        std::sort(v4.begin(), v4.end());
    }



    return 0;
}
