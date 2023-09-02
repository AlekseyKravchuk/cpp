#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional> // std::optional
#include <set>
#include <string>
#include <tuple>
#include <utility> // std::move, std::exchange
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

        return id;
    }

    // Добавить все элементы с нулевыми приоритетами из диапазона [range_begin, range_end) с помощью перемещения,
    // записав выданные им идентификаторы в диапазон [ids_begin, ...)
    // в "range_begin" лежит итератор, указывающий на начало соответствующего диапазона, в котором содержатся элементы типа T
    /*
    метод вызывается так :
        std::vector<T> arr;
        std::vector<Id> dest;
        container.Add(arr.begin(),err.end(), std::back_inserter(dist));
    */
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        auto it = range_begin;

        for (; it != range_end; ++it) {
            auto id = Add(*std::make_move_iterator(it));
            *ids_begin = id;
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
        auto pos = _id_to_pos[id];
        ++_pq[pos].priority;
        FixUp(pos);
    }

    // Получить объект с максимальным приоритетом и его приоритет
    // !!! ПРЕДПОЛАГАЕТСЯ (по условию задачи), что метод "GetMax" вызывается только при наличии элементов в контейнере!!!
    std::pair<const T&, int> GetMax() const {
        return {_pq.at(0).obj, _pq.at(0).priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    // !!! ПРЕДПОЛАГАЕТСЯ (по условию задачи), что метод "PopMax" вызывается только при наличии элементов в контейнере!!!
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

        bool operator==(const TAux& other) {
            bool ok1 = std::tuple(id, priority) == std::tuple(other.id, other.priority);
            bool ok2 = (*this == other);
            return ok1 && ok2;

            // return std::tuple(id, priority) == std::tuple(other.id, other.priority);
        }
    };

    std::vector<TAux> _pq;        // "priority queue" реализованная через "max-heap"
    std::map<Id, int> _id_to_pos; // словарь, позволяющий задать соответствие "id"==>"позиция_в_векторе_pq" + проверка наличия объекта с данным "id" в контейнере

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

        while (i > 0 && _pq[i_parent].priority < _pq[i].priority) {
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
            if ((j < (N - 1)) && (_pq[j].priority < _pq[j + 1].priority)) {
                ++j; // правый ребенок имеет БОЛЕЕ ВЫСОКИЙ ПРИОРИТЕТ, перемещаем индекс на него
            }

            if (_pq[i].priority >= _pq[j].priority) {
                break; // свойство max-кучи более не нарушается (приоритет текущего узла больше, чем максимальный приоритет его детей), выходим из цикла
            }

            SwapTAux(_pq[i], _pq[j]); // меняем местами текущий объект с одним из дочерних, имеющий НАИБОЛЬШИЙ приоритет

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

// void TestAddRange() {
//     {
//         PriorityCollection<StringNonCopyable> strings;
//         const auto white_id = strings.Add("white");
//         const auto yellow_id = strings.Add("yellow");
//         const auto red_id = strings.Add("red");

//         std::vector<StringNonCopyable> src(3);
//         strings.Add(src.begin(), src.end(), strings.begin());

//         std::cout << "At the end of TestAddRange()" << std::endl;
//     }
// }

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);

    return 0;
}
