/*
Пожалуйста, ознакомьтесь с классом StringSet, на разборе которого основано следующее видео.

Этот класс позволяет:
    - добавить строку с заданным приоритетом с помощью метода Add;
    - вернуть последнюю добавленную строку с помощью метода FindLast;
    - вернуть строку с максимальным приоритетом с помощью метода FindBest.
*/

#include <deque>
#include <iostream>
#include <set>
#include <vector>

#include "test_runner.h"

using namespace std::literals;

class StringSet {
   public:
    // добавляет строку с заданным приоритетом в хранилище
    void Add(const std::string& s, int priority) {
        _data.push_back(s);  //  в вектор мы всё так же вставляем исходную строчку

        // теперь во множество нужно складывать индекс последнего элемента в векторе
        _sorted_data.insert({_data.size() - 1u, priority});
    }

    // возвращает последнюю добавленную строку
    const std::string& FindLast() const {
        return _data.back();
    }

    // возвращает строку с максимальным приоритетом
    const std::string& FindBest() const {
        return _data[prev(_sorted_data.end())->index];
    }

   private:
    struct StringItem {
        size_t index;  // вместо ссылки на стоку в векторе сохраняем индекс строки в векторе "_data"
        int priority;

        // перегружаем оператор "less" для того, чтобы иметь возможность хранить объекты типа StringItem в set'e
        bool operator<(const StringItem& other) const {
            return priority < other.priority;
        }
    };

    // линейный контейнер (сохраняет порядок вставки), инвалидирующий ссылки на свои элементы (при добавлении элементов)
    // хотим оставить в качестве контейнера std::vector, поэтому вместо ссылок нужно использовать индексы элементов в векторе
    std::vector<std::string> _data;

    std::set<StringItem> _sorted_data;  // НЕлинейный контейнер (НЕ сохраняет порядок вставки)
};

// =============== Юнит-тесты ===============

void TestSimple() {
    StringSet strings;
    strings.Add("upper"s, 10);
    strings.Add("lower"s, 0);
    ASSERT_EQUAL(strings.FindLast(), "lower"s);
    ASSERT_EQUAL(strings.FindBest(), "upper"s);
}

void TestAnother() {
    StringSet strings;

    strings.Add("apple"s, 1);
    strings.Add("orange"s, 3);
    ASSERT_EQUAL(strings.FindLast(), "orange"s);
    ASSERT_EQUAL(strings.FindBest(), "orange"s);

    strings.Add("plum"s, -5);
    ASSERT_EQUAL(strings.FindLast(), "plum"s);
    ASSERT_EQUAL(strings.FindBest(), "orange"s);

    strings.Add("lemon"s, 5);
    ASSERT_EQUAL(strings.FindLast(), "lemon"s);
    ASSERT_EQUAL(strings.FindBest(), "lemon"s);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    RUN_TEST(tr, TestAnother);

    // StringSet strings;
    // strings.Add("upper"s, 10);
    // strings.Add("lower"s, 0);
    // std::cout << strings.FindLast() << '\n';
    // std::cout << strings.FindBest() << '\n';

    return 0;
}