#include <algorithm>
#include <cstding>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "test_runner.h"

template <typename T>
class PriorityCollection {
   public:
    using Id = uint32_t;
    using Priority = utin32_t;

    // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        _max_heap.push_back(std::move(object));
    }

    // Добавить все элементы диапазона [range_begin, range_end) с помощью перемещения,
    // записав выданные им идентификаторы в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        std::move(range_begin, range_end, std::back_inserter(_max_heap));
    }

    // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return 0 <= id <= _max_heap.size() - 1;
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _max_heap[id];
    }

    // Увеличить приоритет объекта на 1
    // TODO: еще раз посмотреть устройство кучи у Саши Куликова
    void Promote(Id id);

    // Получить объект с максимальным приоритетом и его приоритет
    std::pair<const T&, int> GetMax() const {
        return {_max_heap.at(0).first, _max_heap.at(0).second};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    // TODO:
    std::pair<T, int> PopMax();

   private:
    std::vector<std::pair<T, Priority>> _max_heap;
};

class StringNonCopyable : public std::string {
   public:
    using std::string::string;  // Позволяет использовать конструкторы строки
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

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);

    return 0;
}
