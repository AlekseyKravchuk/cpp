#include <algorithm>  // std::shuffle, std::all_of
#include <cmath>      // std::abs
#include <future>
#include <iterator>
#include <mutex>
#include <numeric>      // std::iota
#include <random>       // std::default_random_engine
#include <string>       // std::to_string
#include <type_traits>  // std::is_integral_v
#include <vector>

#include "profile.h"
#include "test_runner.h"

template <typename K, typename V>
class ConcurrentMap {
   public:
    static_assert(std::is_integral_v<K>, "ConcurrentMap supports only integer keys");

    struct Access {
        std::lock_guard<std::mutex> guard;
        V& ref_to_value;
    };

    struct Bucket {
        V& Get(const K& key, std::mutex& m) {
            std::lock_guard<std::mutex> g(m);
            return bucket[key];
        }

        std::map<K, V> bucket;
    };

    explicit ConcurrentMap(size_t bucket_count) : bucket_count_(bucket_count) {}

    Access operator[](const K& key) {
        size_t bucket;
        bucket = key % bucket_count_;
        
        return {data[bucket].Get(key, mutex_[bucket])};
    }

    std::map<K, V> BuildOrdinaryMap() {
        std::map<K, V> result;

        for (const auto& b : data) {
            for (const auto& item : b.second.bucket) {
                result[item.first] = operator[](item.first).ref_to_value;
            }
        }

        return result;
    }

   private:
    size_t bucket_count_;
    std::map<size_t, Bucket> data;
    std::map<size_t, std::mutex> mutex_;
};

void RunConcurrentUpdates(
    ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
    auto kernel = [&cm, key_count](int seed) {
        std::vector<int> updates(key_count);
        std::iota(std::begin(updates), std::end(updates), -key_count / 2);
        std::shuffle(std::begin(updates), std::end(updates), std::default_random_engine(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                cm[key].ref_to_value++;
            }
        }
    };

    std::vector<std::future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(std::async(kernel, i));
    }
}

void TestConcurrentUpdate() {
    const size_t thread_count = 3;
    const size_t key_count = 50000;

    ConcurrentMap<int, int> cm(thread_count);
    RunConcurrentUpdates(cm, thread_count, key_count);

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto& [k, v] : result) {
        AssertEqual(v, 6, "Key = " + std::to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, std::string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value += 'a';
        }
    };

    auto reader = [&cm] {
        std::vector<std::string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = std::async(updater);
    auto r1 = std::async(reader);
    auto u2 = std::async(updater);
    auto r2 = std::async(reader);

    u1.get();
    u2.get();

    for (auto f : {&r1, &r2}) {
        auto result = f->get();
        ASSERT(std::all_of(result.begin(), result.end(), [](const std::string& s) {
            return s.empty() || s == "a" || s == "aa";
        }));
    }
}

void TestSpeedup() {
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
}
