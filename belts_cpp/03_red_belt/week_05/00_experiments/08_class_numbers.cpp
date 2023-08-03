#include <algorithm>
#include <list>
#include <vector>

#include "../../log_duration.h"

class NumbersOnVector {
   public:
    void Add(int x) {
        _data.push_back(x);
    }

    template <typename Predicate>
    void Remove(Predicate predicate) {
        // std::remove_if физически не удаляет элементы, а просто перемещает их в конец вектора
        // и возвращает итератор на первый элемент диапазона, подлежащего удалению
        _data.erase(std::remove_if(std::begin(_data), std::end(_data), predicate),
                    std::end(_data));
    }

   private:
    std::vector<int> _data;
};

class NumbersOnList {
   public:
    void Add(int x) {
        _data.push_back(x);
    }

    template <typename Predicate>
    void Remove(Predicate predicate) {
        _data.remove_if(predicate);
    }

   private:
    std::list<int> _data;
};

// // ======= В такой конфигурации более эффективен VECTOR =======
// const int SIZE = 1'000'000;
// const int REMOVAL_COUNT = 10;
// // ============================================================


//  // ======= А в такой конфигурации более эффективен LIST ======
const int SIZE = 10'000;
const int REMOVAL_COUNT = 1'000;
// // ============================================================

int main() {
    {
        LOG_DURATION("vector");
        NumbersOnVector numbers;
        for (int i = 0; i < SIZE; ++i) {
            numbers.Add(i);
        }

        for (int i = 0; i < REMOVAL_COUNT; ++i) {
            // удаляем все элементы, у которых остаток от деления на "REMOVAL_COUNT" равен "i"
            numbers.Remove([i](int x) {
                return x % REMOVAL_COUNT == i;
            });
        }
    }

    {
        LOG_DURATION("list");
        NumbersOnList numbers;
        for (int i = 0; i < SIZE; ++i) {
            numbers.Add(i);
        }

        for (int i = 0; i < REMOVAL_COUNT; ++i) {
            // удаляем все элементы, у которых остаток от деления на "REMOVAL_COUNT" равен "i"
            numbers.Remove([i](int x) {
                return x % REMOVAL_COUNT == i;
            });
        }
    }

    return 0;
}
