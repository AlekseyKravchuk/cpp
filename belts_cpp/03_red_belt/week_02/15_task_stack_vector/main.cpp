#include <random>

#include "log_duration.h"
#include "stack_vector.h"
#include "test_runner.h"
#include "tests.h"

int main() {
    {
        TestRunner tr;
        RUN_TEST(tr, TestConstruction);
        RUN_TEST(tr, TestPushBack);
        RUN_TEST(tr, TestPopBack);
    }

    std::cerr << "Running benchmark..." << std::endl;
    const size_t max_size = 2500;

    std::default_random_engine re;
    std::uniform_int_distribution<int> value_gen(1, max_size);

    std::vector<std::vector<int>> test_data(50000);
    for (auto& cur_vec : test_data) {
        cur_vec.resize(value_gen(re));
        for (int& x : cur_vec) {
            x = value_gen(re);
        }
    }

    {
        LOG_DURATION("vector w/o reserve");
        for (auto& cur_vec : test_data) {
            std::vector<int> v;
            for (int x : cur_vec) {
                v.push_back(x);
            }
        }
    }
    {
        LOG_DURATION("vector with reserve");
        for (auto& cur_vec : test_data) {
            std::vector<int> v;
            v.reserve(cur_vec.size());
            for (int x : cur_vec) {
                v.push_back(x);
            }
        }
    }
    {
        LOG_DURATION("StackVector");
        for (auto& cur_vec : test_data) {
            StackVector<int, max_size> v;
            for (int x : cur_vec) {
                v.PushBack(x);
            }
        }
    }
    std::cerr << "Done" << std::endl;
}
