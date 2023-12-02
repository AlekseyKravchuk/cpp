#include <algorithm>
#include <iostream>
#include <iterator> // std::make_reverse_iterator
#include <numeric>  // std::iota
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility> // std::move, std::exchange, std::pair
#include <vector>

#include "test_runner.h"

template <typename T>
class PriorityCollection {
  public:
    using Id = int;
    using Priority = int;

    // Добавить объект с нулевым приоритетом с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        Id id = static_cast<Id>(_pq.size());
        _pq.push_back({id,
                       Priority{0},
                       std::move(object)});
        _id_to_pos[id] = id; // изначально при добавлении объекта его "id" соответствует его позиции (0-based index) в max-heap'e (вектор _pq)

        // Здесь метод "FixUp" вызывается для случая, когда элементы в max-heap имеют одинаковые приоритеты, но разные "id"
        // Согласно условию, при наличии нескольких элементов с одинаковым максимальным приоритетом необходимо выбирать и извлекать тот,
        // который был добавлен позже остальных. Поэтому узел с МАКСИМАЛЬНЫМ приоритетом И МАКСИМАЛЬНЫМ "id" должен оказаться в корне max-heap
        FixUp(id);

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
    // в результате вызова метода будет добавлено "(range_end - range_begin)" элементов с нулевым приоритетом
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        if (range_begin == range_end) {
            return;
        }

        // Идентификаторы будут назначаться в обратном порядке в порядке уменьшения (от бОльших значений "id" к меньшим),
        // поскольку элементы будут добавляться в max_heap с помощью reverse_iterator'ов.
        int pos = static_cast<Id>(_pq.size());                                     // начальная позиция в max-heap'e для вставки 1-го эл-та из диапазона
        Id id = pos + static_cast<int>(std::distance(range_begin, range_end)) - 1; // максимальный индекс для добавляемого диапазона

        auto rbegin = std::make_reverse_iterator(range_end);
        auto rend = std::make_reverse_iterator(range_begin);

        for (; rbegin != rend; ++rbegin) {
            _pq.push_back({id,
                           Priority{0},
                           *std::make_move_iterator(rbegin)});
            _id_to_pos[id--] = pos; // изначально позиция объекта в куче соответствует его "id"
            *ids_begin++ = pos++;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return _id_to_pos.count(id);
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _pq.at(_id_to_pos.at(id)).obj;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        int pos = _id_to_pos[id];

        ++_pq[pos].priority;
        FixUp(pos);
    }

    // Получить объект с максимальным приоритетом и его приоритет. "GetMax" вызывается только при наличии элементов в контейнере!!!
    std::pair<const T&, int> GetMax() const {
        return {_pq.at(0).obj, _pq.at(0).priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера. "PopMax" вызывается только при наличии элементов в контейнере!!!
    std::pair<T, int> PopMax() {
        SwapTAux(_pq[0], _pq.back());     // меняем местами 0-ой (с max-приоритетом) и последний (возможно, с min-приоритетом) элементы в max-heap
        Id id_to_del = _pq.back().id;     // запоминаем "id", подлежащий удалению
        TAux tmp = std::move(_pq.back()); // в "tmp" теперь объект типа "TAux" c наивысшим приоритетом
        _pq.pop_back();                   // избавляемся от последнего элемента в max-heap
        _id_to_pos.erase(id_to_del);      // удаляем идентификатор из "_id_to_pos"

        // восстанавливаем свойство кучи, которое, возможно, было нарушено
        FixDown(0); // просеиваем вниз объект, который встал вместо объекта с наивысшим приоритетом

        return std::pair<T, int>{std::move(tmp.obj), tmp.priority};
    }

  private:
    struct TAux {
        Id id;             // уникальный идентификатор для каждого объекта типа "T"
        Priority priority; // приоритет объекта
        T obj;             // объект типа T

        bool operator==(const TAux& other) const {
            return std::tuple(priority, id) == std::tuple(other.priority, other.id);
        }

        bool operator<(const TAux& other) const {
            return std::tuple(priority, id) < std::tuple(other.priority, other.id);
        }

        bool operator>=(const TAux& other) const {
            return std::tuple(priority, id) >= std::tuple(other.priority, other.id);
        }
    };

    std::vector<TAux> _pq;                  // "priority queue" реализованная через "max-heap"
    std::unordered_map<Id, int> _id_to_pos; // словарь, "id"==>"позиция_в_куче" + проверка наличия объекта с данным "id" в контейнере

    // ====================================== private Methods ======================================
    void SwapTAux(TAux& lhs, TAux& rhs) {
        if (lhs == rhs) {
            return;
        }
        std::swap(_id_to_pos[lhs.id], _id_to_pos[rhs.id]); // вносим необходимые изменения в словарь соответствий "id=>pos_in_max_heap"
        std::swap(lhs, rhs);                               // меняем местами объекты типа "TAux"
    }

    // Метод позволяет восстановить свойство кучи - вершина, сломавшая инвариант max-кучи "всплывает" наверх к корню дерева.
    // Такая необходимость возникает в следующих 2-х случаях:
    //    1) был УВЕЛИЧЕН приоритет некоторой вершины в результате чего приоритет ребенка стал выше приоритета родителя;
    //    2) был добавлен новый узел внизу дерева (в качестве последнего элемента вектора "_max_heap") - теперь он должен занять свое место в куче.
    void FixUp(int i) {
        Id i_parent = (i - 1) / 2;

        while (i > 0 && _pq[i_parent] < _pq[i]) {
            SwapTAux(_pq[i_parent], _pq[i]);
            i = std::exchange(i_parent, (i_parent - 1) / 2);
        }
    }

    // Метод позволяет восстановить свойство кучи - вершина, сломавшая инвариант max-кучи "просеивается" вниз.
    // Такая необходимость возникает после вызова метода "PopMax()", когда приоритет объекта "TAux" в корне дерева УМЕНЬШАЕТСЯ
    // в результате обмена 0-го и последнего элементов "max-heap'a" и становится меньше чем приоритет одного или сразу обоих его детей.
    // Мы можем исправить "поломку", просто поменяв объект, хранящийся в текущем узле (с индексом "i"), на ребенка с НАИБОЛЬШИМ приоритетом.
    // Этот обмен может привести к нарушению свойства кучи уже в дочернем узле.
    // Таким образом, необходимо "чинить" нарушение свойства кучи до тех пор, пока:
    //    - мы не достигнем узла, приоритет которого, ВЫШЕ, чем приоритеты обоих его детей;
    //    - или пока мы не дойдем до листа.
    void FixDown(Id i) {                      // i - индекс текущего узла
        int N = static_cast<int>(_pq.size()); // общее количество элементов в max-heap'e
        Id j = 2 * i + 1;                     // индекс "left_child", потом, возможно, случится инкремент и это будет индекс "right_child"

        while (j < N) {
            // Проверка "(j < (N - 1))" нужна для случая, когда N - ЧЕТНОЕ И "i == N/2"
            // В этом случае текущий узел имеет РОВНО ОДНОГО ребенка (левого).
            // Т.е. проверка нужна, чтобы не выйти за границы вектора в результате "j+1"
            if ((j < (N - 1)) && (_pq[j] < _pq[j + 1])) {
                ++j; // правый ребенок имеет БОЛЕЕ ВЫСОКИЙ ПРИОРИТЕТ, перемещаем индекс на него
            }

            if (_pq[i] >= _pq[j]) {
                break; // свойство max-кучи более не нарушается (приоритет текущего узла больше, чем максимальный приоритет его детей), выходим из цикла
            }

            SwapTAux(_pq[i], _pq[j]); // меняем местами текущий объект с одним из дочерних, имеющим НАИБОЛЬШИЙ приоритет

            // теперь индекс "i" указывает на ребенка с наибольшим приоритетом, а "j" на левого ребенка текущего узла
            i = std::exchange(j, 2 * j + 1);
        }
    }
    // ================================ END of private Methods ================================
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

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestAddRange);
    RUN_TEST(tr, TestEqualIDs);

    return 0;
}
