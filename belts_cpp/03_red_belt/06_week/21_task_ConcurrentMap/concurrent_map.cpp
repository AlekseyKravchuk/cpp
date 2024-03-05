#include <algorithm> // std::shuffle, std::all_of
#include <cmath>     // std::abs
#include <future>
#include <iterator>
#include <map>
#include <mutex>
#include <numeric>     // std::iota
#include <random>      // std::default_random_engine
#include <string>      // std::to_string
#include <type_traits> // std::is_integral_v
#include <vector>

#include "profile.h"
#include "test_runner.h"

template <typename KeyType, typename ValueType>
class ConcurrentMap {
  private:
    struct Bucket {
        std::mutex mutex;
        std::map<KeyType, ValueType> dict;
    };

  public:
    static_assert(std::is_integral_v<KeyType>, "ConcurrentMap supports only integer keys");

    struct Access {
        std::lock_guard<std::mutex> guard;
        ValueType& ref_to_value;

        Access(Bucket& bucket, const KeyType& key)
            : guard(bucket.mutex),
              ref_to_value(bucket.dict[key]) {}
    };

    explicit ConcurrentMap(size_t bucket_count) : _buckets(bucket_count) {}

    Access operator[](const KeyType& key) {
        auto& bucket = _buckets[static_cast<uint64_t>(key) % _buckets.size()];
        return Access{bucket, key};
    }

    std::map<KeyType, ValueType> BuildOrdinaryMap() {
        std::map<KeyType, ValueType> result;

        for (auto& [mutex, dict] : _buckets) {
            std::lock_guard g(mutex);
            result.insert(dict.begin(), dict.end());
        }

        return result;
    }

  private:
    std::vector<Bucket> _buckets;
};

// // ===========================================================================================
// // ========================================== TESTS ==========================================
// // ===========================================================================================
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
// // ===================================== END OF TESTS =====================================

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
}
