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

    // Структура Access, должна вести себя так же, как и в шаблоне Synchronized,
    // то есть предоставлять ссылку на ЗНАЧЕНИЕ словаря и обеспечивать синхронизацию доступа к нему.
    struct Access {
        std::lock_guard<std::mutex>& guard;
        V& ref_to_value;
    };

    // Конструктор класса ConcurrentMap<K, V> принимает количество подсловарей, на которые надо разбить всё пространство ключей.
    explicit ConcurrentMap(size_t N)
        : _range(Range<K>{}),
          _N_buckets(N),
          _subdict_store(N),
          _guards(N),
          _bucket_indexer(SetBucketIndexer()) {}

    // operator[] должен вести себя так же, как аналогичный оператор у map — если ключ key присутствует в словаре,
    // он должен возвращать объект класса Access, содержащий ссылку на соответствующее ему значение;
    // если же key отсутствует в словаре, в него надо добавить пару (key, V()) и вернуть объект класса Access,
    // содержащий ссылку на только что добавленное значение.
    Access operator[](const K& key) {
        size_t i = GetSubDictId(key);  // сначала находим "subdict", в котором теоретически может содержаться ключ "key"
        std::lock_guard<std::mutex> guard(_guards[i]);

        // упрощаем логику возвращаемого значения для отладочных целей
        return Access{guard, _subdict_store[i][key]};  // <= error: use of deleted function
                                                       // ‘std::lock_guard<_Mutex>::lock_guard(const std::lock_guard<_Mutex>&) [with _Mutex = std::mutex]’

        // if (_subdict_store[i].count(key)) {
        //     return Access{guard, _subdict_store[i].at(key)};
        // } else {
        //     auto [it, success] = _subdict_store[i].insert({key, V()});
        //     return {guard, *it};
        // }
    }

    // Метод BuildOrdinaryMap должен сливать вместе части словаря и возвращать весь словарь целиком.
    // При этом он должен быть потокобезопасным, то есть корректно работать, когда другие потоки выполняют операции с ConcurrentMap.
    std::map<K, V> BuildOrdinaryMap() {
        if (!_subdict_store.size()) {
            return {};
        }

        if (_subdict_store.size() == 1ul) {
            return _subdict_store[0];
        }

        std::mutex mtx;
        std::map<K, V> merged_map;
        {
            std::lock_guard<std::mutex> guard(mtx);

            size_t i = 0;
            for (auto subdict : _subdict_store) {
                std::lock_guard<std::mutex> guard(_guards[i++]);
                merged_map.merge(subdict);
            }
        }

        return merged_map;
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

   private:
    const Range<K> _range;
    size_t _N_buckets;
    std::vector<std::map<K, V>> _subdict_store;
    std::vector<std::mutex> _guards;
    std::map<int, int> _bucket_indexer;

   private:  // ===================== Private Methods =====================
    size_t GetNumElementsPerBucket() {
        return (_range.size % _N_buckets == 0)
                   ? _range.size / _N_buckets
                   : _range.size / _N_buckets + 1;
    }

    std::map<int, int> SetBucketIndexer() {
        std::map<int, int> bucket_indexer;
        int border = _range.min;
        int elements_per_bucket = GetNumElementsPerBucket();

        for (size_t i = 0; i < _N_buckets; ++i) {
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
