#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric> // std::iota
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "test_runner.h"

template <typename T>
class PriorityCollection {
  public:
    using Id = int;
    using Priority = int;

    // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
    Id Add(T data) {
        const Id id = static_cast<Id>(_objects.size());
        _objects.push_back({std::move(data), Priority{}});
        _sorted_objects.insert(std::pair<Priority, Id>{0, id});

        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end) с помощью перемещения,
    // записав выданные им идентификаторы в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        while (range_begin != range_end) {
            *ids_begin++ = Add(std::move(*range_begin++));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return id >= 0 && id < static_cast<Id>(_objects.size()) && _objects[id].priority != NONE_PRIORITY;
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _objects[id].data;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        Object& obj = _objects[id];
        if (auto nh = _sorted_objects.extract({obj.priority, id}); !nh.empty()) {
            nh.value() = std::pair{++obj.priority, id};
            _sorted_objects.insert(std::move(nh));
        }
    }

    // Получить объект с максимальным приоритетом и его приоритет
    std::pair<const T&, int> GetMax() const {
        Id id = std::prev(_sorted_objects.end())->second;
        const Object& obj = _objects[id];
        return {obj.data, obj.priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    std::pair<T, int> PopMax() {
        const auto it_sorted_objects = std::prev(_sorted_objects.end());
        Id id = it_sorted_objects->second;
        _sorted_objects.erase(it_sorted_objects);

        Object& obj = _objects[id];
        const int priority = obj.priority;
        obj.priority = NONE_PRIORITY;

        return {std::move(obj.data), priority};
    }

  private:
    struct Object {
        T data;
        Priority priority{0};
    };

    static const int NONE_PRIORITY = -1;
    std::vector<Object> _objects;
    std::set<std::pair<Priority, Id>> _sorted_objects;
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
