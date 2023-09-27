#include <algorithm>
#include <cstdlib>
#include <future>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "profile.h"
#include "test_runner.h"

using namespace std::string_literals;

template <typename Key, typename Value>
class ConcurrentMap {
   public:
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys");

    explicit ConcurrentMap(size_t bucket_count) : _buckets(bucket_count),
                                                  _guards(bucket_count),
                                                  _parts(bucket_count) {
    }

    struct Access {
        // Access(std::lock_guard<std::mutex>& guard, Value& value) : _guard(guard), ref_to_value(value) {
        // }

        std::lock_guard<std::mutex>& _guard;
        Value& ref_to_value;
    };

    Access operator[](const Key& key) {
        std::lock_guard<std::mutex> guard(_guards[key % _buckets]);  // заблокировал доступ к корзине, в которой лежит нужный словарь

        return {guard, _parts[key % _buckets][key]};  // взял из этого словаря значение, находясь под защитой guard
    }

    std::map<Key, Value> BuildOrdinaryMap() {
        // std::map<Key, Value> result;
        // for (size_t i = 0; i < _parts.size(); ++i) {
        //     std::lock_guard<std::mutex> guard(_guards[i]);  // заблокировал корзину, в которой лежит мапа
        //     for (auto& [k, v] : _parts[i]) {                // заюираю из корзины все, находясь под защитой guard
        //         result[k] = v;
        //     }
        // }

        if (!_parts.size()) {
            return {};
        }

        if (_parts.size() == 1ul) {
            return _parts[0];
        }

        std::mutex mtx;
        std::map<Key, Value> merged_map;
        {
            std::lock_guard<std::mutex> guard(mtx);
            size_t i = 0;
            
            for (auto subdict : _parts) {
                std::lock_guard<std::mutex> guard(_guards[i++]);
                merged_map.merge(subdict);
            }
        }

        return merged_map;

        // return result;
    }

   private:
    size_t _buckets;
    std::vector<std::mutex> _guards;
    std::vector<std::map<Key, Value>> _parts;
};

using namespace std;

void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
    auto kernel = [&cm, key_count](int seed) {
        vector<int> updates(key_count);
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), mt19937(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                ++cm[key].ref_to_value;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(async(kernel, i));
    }
}

void TestConcurrentUpdate() {
    constexpr size_t THREAD_COUNT = 3;
    constexpr size_t KEY_COUNT = 50000;

    ConcurrentMap<int, int> cm(THREAD_COUNT);
    RunConcurrentUpdates(cm, THREAD_COUNT, KEY_COUNT);

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), KEY_COUNT);
    for (auto& [k, v] : result) {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value.push_back('a');
        }
    };
    auto reader = [&cm] {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : {&r1, &r2}) {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
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