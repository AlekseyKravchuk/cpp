#include <numeric> // std::iota
#include <set>
#include <string>
#include <unordered_map>
#include <utility> // std::pair, std::move

#include "test_runner.h"

template <typename T>
class PriorityCollection {
  public:
    using Id = int;
    using Priority = int;
    using RankingSet = std::set<std::pair<Priority, Id>, std::greater<std::pair<Priority, Id>>>;
    using IndexerMap = std::unordered_map<Id, std::pair<Priority, Id>>;

    // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        Id id = _next_id_to_assign++;
        _storage.insert({id, std::move(object)});
        _ranking.insert({Priority{0}, id});
        _indexer.insert({id, std::pair{Priority{0}, id}});

        return id;
    }

    // Добавить все элементы с нулевыми приоритетами из диапазона [range_begin, range_end) с помощью перемещения,
    // записав выданные им идентификаторы в диапазон [ids_begin, ...)
    // в "range_begin" лежит итератор, указывающий на начало соответствующего диапазона, в котором содержатся элементы типа T
    /*
    метод вызывается так :
        std::vector<T> arr;
        std::vector<Id> dest;
        container.Add(arr.begin(),arr.end(), std::back_inserter(dest));
    */
    // Будет добавлено "(range_end - range_begin)" элементов с нулевым приоритетом,
    // объекты будут перемещаться в "_storage" в порядке их следования в диапазоне [range_begin, range_end)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        if (range_begin == range_end) {
            return;
        }

        auto it = range_begin;
        for (; it != range_end; ++it) {
            *ids_begin++ = _next_id_to_assign;
            Add(*std::make_move_iterator(it));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return _storage.count(id);
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _storage.at(id);
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        auto nh = _ranking.extract(_indexer[id]);
        nh.value().first += 1;
        _indexer[id] = nh.value();
        _ranking.insert(std::move(nh));
    }

    // Получить объект с максимальным приоритетом и его приоритет, "GetMax" вызывается только при наличии элементов в контейнере
    std::pair<const T&, int> GetMax() const {
        const auto [priority, id] = *_ranking.begin();
        return {_storage.at(id), priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    std::pair<T, int> PopMax() {
        auto& [priority, id] = *_ranking.begin();
        T obj = std::move(_storage[id]);
        _ranking.erase(_indexer[id]);
        _indexer.erase(id);
        _storage.erase(id);

        return std::pair<T, int>{std::move(obj), priority};
    }

  private:
    RankingSet _ranking;                // set< pair<Priority, Id> > для вычисления ранга по паре <priority, id>
    IndexerMap _indexer;                // unordered_map<Id, pair<Priority,Id> >, словарь, задающий соответствие "id : pair<Priority, Id>"
    std::unordered_map<Id, T> _storage; // unordered_map<Id, T>, фактическое хранилище объектов
    Id _next_id_to_assign{0};           // следующий доступный "id"
};

class StringNonCopyable : public std::string {
  public:
    using std::string::string; // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);

    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }

    strings.Promote(yellow_id);

    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }

    {
        const auto item = strings.PopMax(); // <= некорректно работает PopMax
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2); // <= падаем теперь здесь
    }

    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

void TestEqualIDs() {
    {
        PriorityCollection<int> collection;
        std::vector<int> arr{1, 2, 3, 4, 5, 6};
        std::vector<int> ids;

        collection.Add(arr.begin(), arr.end(), std::back_inserter(ids));

        for (int i = 1; i < 4; ++i) {
            collection.Promote(i);
        }

        std::pair<int, int> elm = collection.PopMax();
        ASSERT_EQUAL(elm, (std::pair<int, int>(4, 1)));

        elm = collection.PopMax();
        ASSERT_EQUAL(elm, (std::pair<int, int>{3, 1}));

        elm = collection.PopMax();
        ASSERT_EQUAL(elm, (std::pair<int, int>{2, 1}));

        elm = collection.PopMax();
        ASSERT_EQUAL(elm, (std::pair<int, int>{6, 0}));

        elm = collection.PopMax();
        ASSERT_EQUAL(elm, (std::pair<int, int>{5, 0}));

        elm = collection.PopMax();
        ASSERT_EQUAL(elm, (std::pair<int, int>{1, 0}));

        // for (size_t i = 0; i < ids.size(); ++i) {
        //     auto elm = collection.PopMax();
        //     std::cout << elm.first << ' ' << elm.second << '\n';
        // }
    }
}

void TestAddRange() {
    using PrCollect = PriorityCollection<int>;
    using Id = PrCollect::Id;
    int const SIZE = 10;

    std::vector<Id> ids(SIZE);
    std::vector<int> range(SIZE);
    std::iota(range.begin(), range.end(), 0);

    PrCollect collection;
    collection.Add(range.begin(), range.end(), ids.begin());

    int prevMax = SIZE - 1;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            collection.Promote(ids[i]);
            auto [item, priority] = collection.GetMax();
            int trueValue = j + 1 >= i ? i : prevMax;
            int truePriority = j + 1 >= i ? j + 1 : i;
            ASSERT(item == trueValue && priority == truePriority);
        }
        prevMax = i;
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestAddRange);
    RUN_TEST(tr, TestEqualIDs);

    return 0;
}
