#include <iterator> // std::prev
#include <numeric>  // std::iota
#include <set>
#include <string>
#include <unordered_map>
#include <utility> // std::pair, std::move

#include "test_runner.h"

template <typename T>
class PriorityCollection {
  public:
    using Id = int;

    Id Add(T object) {
        const Id new_id = _objects.size();
        _objects.push_back({std::move(object)});
        _sorted_objects.insert({0, new_id});
        return new_id;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        while (range_begin != range_end) {
            *ids_begin++ = Add(std::move(*range_begin++));
        }
    }

    bool IsValid(Id id) const {
        return id >= 0 && id < _objects.size() &&
               _objects[id].priority != NONE_PRIORITY;
    }

    const T& Get(Id id) const {
        return _objects[id].data;
    }

    void Promote(Id id) {
        auto& item = _objects[id];
        const int old_priority = item.priority;
        const int new_priority = ++item.priority;
        _sorted_objects.erase({old_priority, id});
        _sorted_objects.insert({new_priority, id});
    }

    std::pair<const T&, int> GetMax() const {
        const auto& item = _objects[std::prev(_sorted_objects.end())->second];
        return {item.data, item.priority};
    }

    std::pair<T, int> PopMax() {
        const auto sorted_objects_it = std::prev(_sorted_objects.end());
        auto& item = _objects[sorted_objects_it->second];
        _sorted_objects.erase(sorted_objects_it);
        const int priority = item.priority;
        item.priority = NONE_PRIORITY;
        return {std::move(item.data), priority};
    }

  private:
    struct ObjectItem {
        T data;
        int priority = 0;
    };

    static const int NONE_PRIORITY = -1;

    std::vector<ObjectItem> _objects;
    std::set<std::pair<int, Id>> _sorted_objects;
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
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }

    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
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

    return 0;
}
