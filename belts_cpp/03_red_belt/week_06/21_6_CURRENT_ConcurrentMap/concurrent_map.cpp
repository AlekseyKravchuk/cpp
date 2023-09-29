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

template <typename KeyType, typename ValueType>
class ConcurrentMap {
   public:
    static_assert(std::is_integral_v<KeyType>, "ConcurrentMap supports only integer keys");

    struct Access {
        std::lock_guard<std::mutex> guard;
        ValueType& ref_to_value;
    };

    struct DictAccess {
        std::lock_guard<std::mutex> guard;
        std::map<KeyType, ValueType>& ref_to_dict;
    };

    explicit ConcurrentMap(size_t num_buckets)
        : _range(Range<KeyType>{}),
          _num_buckets(num_buckets),
          _subdict_store(num_buckets),
          _guards(num_buckets),
          _bucket_indexer(SetBucketIndexer()) {}

    Access operator[](const KeyType& key) {
        // сначала находим индекс "bucket'а", в котором теоретически может содержаться ключ "key"
        size_t i = GetSubDictId(key);
        DictAccess d_acc = _subdict_store[i].GetAccessToSubdict();

        if (d_acc.ref_to_dict.count(key)) {
            return Access{std::lock_guard<std::mutex>(_subdict_store[i].GetRefToMutex()), d_acc.ref_to_dict[key]};
        } else {
            auto [it, success] = d_acc.ref_to_dict.insert({key, ValueType()});
            return Access{std::lock_guard<std::mutex>(_subdict_store[i].GetRefToMutex()), it->second};
        }
    }

    std::map<KeyType, ValueType> BuildOrdinaryMap() {
        Bucket res_bucket;
        DictAccess res_bucket_acc = res_bucket.GetAccessToSubdict();

        for (size_t i = 0; i < _subdict_store.size(); ++i) {
            DictAccess target_bucket_acc = _subdict_store[i].GetAccessToSubdict();
            res_bucket_acc.ref_to_dict.insert(target_bucket_acc.ref_to_dict.begin(), target_bucket_acc.ref_to_dict.end());
        }

        return res_bucket_acc.ref_to_dict;
    }

   private:
    template <typename T>
    struct Range {
        T min{};
        T max{};
        uint64_t size;

        Range(T start_range = std::numeric_limits<T>::min(),
              T end_range = std::numeric_limits<T>::max())
            : min(start_range),
              max(end_range) {
            auto sgn = [](T val) {
                return ((T{} < val) - (val < T{}));
            };

            if (sgn(min) == -1) {
                uint64_t digits = static_cast<uint64_t>(std::numeric_limits<T>::digits);
                size = 2ul << digits;
            } else if (sgn(min) == 0) {
                size = static_cast<uint64_t>(max) + 1ul;
            } else {
                // for user-defined input range for resulting map
                size = static_cast<uint64_t>(max) - static_cast<uint64_t>(min) + 1ul;
            }
        }
    };  // end of Range struct ctor

    struct Bucket {
        std::mutex mtx;
        std::map<KeyType, ValueType> _dict;

        DictAccess GetAccessToSubdict() {
            return {std::lock_guard<std::mutex>(mtx), _dict};
        }

        std::mutex& GetRefToMutex() {
            return mtx;
        }
    };

   private:
    const Range<KeyType> _range;
    size_t _num_buckets;
    std::vector<Bucket> _subdict_store;
    std::vector<std::mutex> _guards;
    std::map<int, int> _bucket_indexer;

   private:  // ===================== Private Methods =====================
    size_t GetNumElementsPerBucket() {
        return (_range.size % _num_buckets == 0)
                   ? _range.size / _num_buckets
                   : _range.size / _num_buckets + 1;
    }

    std::map<int, int> SetBucketIndexer() {
        std::map<int, int> bucket_indexer;
        int border = _range.min;
        int elements_per_bucket = GetNumElementsPerBucket();

        for (size_t i = 0; i < _num_buckets; ++i) {
            bucket_indexer[border] = i;
            border += elements_per_bucket;
        }

        return bucket_indexer;
    }

    // returns index of subdict within "_subdict_store" ("std::vector<Bucket>")
    int GetSubDictId(int key) const {
        return _bucket_indexer.count(key)
                   ? _bucket_indexer.at(key)
                   : std::prev(_bucket_indexer.upper_bound(key))->second;
    }
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
