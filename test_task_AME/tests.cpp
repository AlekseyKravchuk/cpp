#include "test_runner.h"
#include "tests.h"
#include "utilities.h"
#include "sorting_algorithms.h"

#include <algorithm>

using namespace std;

void TestCorrectness_CountingSort() {
    vector<int> seeds = {11, 15, 17, 49, 63, 93};

    for (auto seed : seeds) {
        vector<int> v1 = GenerateRandomVectorOfInt(1'000'000, seed);
        CountingSort(v1.begin(), v1.end());

        ASSERT(is_sorted(v1.begin(), v1.end()));
    }
}

void TestCorrectness_CountingSort_MultiThread() {
    vector<int> seeds = {11, 15, 17, 49, 63, 93};

    for (auto seed : seeds) {
        vector<int> v1 = GenerateRandomVectorOfInt(1'000'000, seed);
        CountingSort_MultiThread(v1.begin(), v1.end());

        ASSERT(is_sorted(v1.begin(), v1.end()));
    }
}

void TestCorrectness_CountingSort_MultiThread_Merge() {
    vector<int> seeds = {11, 15, 17, 49, 63, 93};

    for (auto seed : seeds) {
        vector<int> v1 = GenerateRandomVectorOfInt(1'000'000, seed);
        CountingSort_MultiThread_Merge(v1.begin(), v1.end());

        ASSERT(is_sorted(v1.begin(), v1.end()));
    }
}


void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestCorrectness_CountingSort);
    RUN_TEST(tr, TestCorrectness_CountingSort_MultiThread);
    RUN_TEST(tr, TestCorrectness_CountingSort_MultiThread_Merge);

}