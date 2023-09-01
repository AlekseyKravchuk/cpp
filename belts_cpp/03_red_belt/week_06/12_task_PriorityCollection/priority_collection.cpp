#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>  // std::optional
#include <set>
#include <string>
#include <utility>  // std::move, std::exchange
#include <vector>

#include "test_runner.h"

template <typename T>
class PriorityCollection {
   public:
    using Id = int;
    using Priority = int;

    // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        _max_heap.push_back({std::move(object), 0});
        Id id = static_cast<Id>(_max_heap.size()) - 1;
        _id_to_pos[id] = id;  // при добавлении элемента его "id" соответствует его позиции в max-heap'e

        return id;
    }

    // Добавить все элементы с нулевыми приоритетами из диапазона [range_begin, range_end) с помощью перемещения,
    // записав выданные им идентификаторы в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        Id id_start = static_cast<Id>(_max_heap.size()) - 1;
        std::move(range_begin, range_end, std::back_inserter(_max_heap));
        Id id_end = static_cast<Id>(_max_heap.size()) - 1;

        while (id_start <= id_end) {
            _id_to_pos[id_start] = id_start;
            ++id_start;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return _id_to_pos.count(id);
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _max_heap.at(_id_to_pos[id]);
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        ++_max_heap[id].second;
        FixUp(id);
    }

    // Получить объект с максимальным приоритетом и его приоритет
    // !!! ПРЕДПОЛАГАЕТСЯ (по условию задачи), что метод "GetMax" вызывается только при наличии элементов в контейнере!!!
    std::pair<const T&, int> GetMax() const {
        return {_max_heap.at(0).first, _max_heap.at(0).second};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    // !!! ПРЕДПОЛАГАЕТСЯ (по условию задачи), что метод "PopMax" вызывается только при наличии элементов в контейнере!!!
    std::pair<T, int> PopMax() {
        if (_max_heap.size() == 1u) {
            auto elm_max_priority = std::move(_max_heap.back());
            _max_heap.pop_back();
            return elm_max_priority;
        }

        std::swap(_max_heap[0], _max_heap.back());  // меняем местами объект с наивысшим приоритетом с последним узлом в векторе
        auto elm_max_priority = std::move(_max_heap.back());
        _max_heap.pop_back();
        FixDown(0);  // просеиваем вниз объект, который встал вместо объекта с наивысшим приоритетом

        return elm_max_priority;
    }

   private:
    std::vector<std::pair<T, Priority>> _max_heap;
    std::map<Id, Id> _id_to_pos;

    // =================== private Methods ===================

    // Метод позволяет восстановить свойство кучи - вершина, сломавшая инвариант max-кучи "всплывает" наверх к корню дерева.
    // Такая необходимость возникает в следующих 2-х случаях:
    //    1) был УВЕЛИЧЕН приоритет некоторой вершины в результате чего приоритет ребенка стал выше приоритета родителя;
    //    2) был добавлен новый узел внизу дерева (в качестве последнего элемента вектора "_max_heap") - теперь он должен занять свое место в куче.
    void FixUp(Id i) {
        // TODO: внести изменения в "_id_to_pos"
        Id i_parent = (i - 1) / 2;

        while ((i > 0) && (_max_heap[i_parent].second < _max_heap[i].second)) {
            // ===> // TODO: написать свой метод Swap
            std::swap(_max_heap[i_parent], _max_heap[i]);
            i = std::exchange(i_parent, (i_parent - 1) / 2);
        }
    }

    // Метод позволяет восстановить свойство кучи - вершина, сломавшая инвариант max-кучи "просеивается" вниз.
    // Такая необходимость возникает, когда приоритет текущего узла был УМЕНЬШЕН И СТАЛ МЕНЬШЕ, чем приоритет одного или сразу обоих его детей.
    // Мы можем исправить "поломку", просто поменяв объект, хранящийся в текущем узле (с индексом "i"), на ребенка с НАИБОЛЬШИМ приоритетом.
    // Этот обмен может привести к нарушению свойства кучи уже в дочернем узле.
    // Таким образом, необходимо "чинить" нарушение свойства кучи до тех пор, пока:
    //    - мы не достигнем узла, приоритет которого, ВЫШЕ, чем приоритеты обоих его детей;
    //    - или пока мы не дойдем до листа.
    //
    void FixDown(Id i) {                             // i - индекс текущего узла
        int N = static_cast<int>(_max_heap.size());  // общее количество элементов в max-heap
        Id j = 2 * i + 1;                            // индекс "left_child", потом, возможно, случится инкремент и это будет индекс "right_child"

        while (j < N) {
            // Проверка "(j < (N - 1))" нужна для случая, когда N - ЧЕТНОЕ И "i == N/2"
            // В этом случае текущий узел имеет РОВНО ОДНОГО ребенка (левого).
            // Т.е. проверка нужна, чтобы не выйти за границы вектора в результате "j+1"
            if ((j < (N - 1)) && (_max_heap[j].second < _max_heap[j + 1].second)) {
                ++j;  // правый ребенок имеет БОЛЕЕ ВЫСОКИЙ ПРИОРИТЕТ, перемещаем индекс на этот объект
            }

            if (_max_heap[i].second >= _max_heap[j].second) {
                break;  // свойство max-кучи более не нарушается, выходим из цикла
            }

            std::swap(_max_heap[i], _max_heap[j]);  // меняем местами текущий объект с одним из дочерних, имеющий НАИБОЛЬШИЙ приоритет

            // теперь индекс "i" указывает на ребенка с наибольшим приоритетом, а "j" на левого ребенка текущего узла
            i = std::exchange(j, 2 * j + 1);
        }
    }

    // ================ END of private Methods ================
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
