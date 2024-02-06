#pragma once

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <vector>

// ======================= IteratorRange template =======================
template <typename Iterator>
class IteratorRange {
  public:
    // Если в классе есть конструктор, позволяющий определить тип шаблонного параметра, компилятор выводит этот тип сам
    IteratorRange(Iterator it_first, Iterator it_last);

    Iterator begin() const;
    Iterator end() const;

  private:
    Iterator _first;
    Iterator _last;
};

template <typename Iterator>
IteratorRange<Iterator>::IteratorRange(Iterator it_first, Iterator it_last)
    : _first(it_first),
      _last(it_last) {}

template <typename Iterator>
Iterator IteratorRange<Iterator>::begin() const {
    return _first;
}

template <typename Iterator>
Iterator IteratorRange<Iterator>::end() const {
    return _last;
}

// // шаблон функции "Head" позволяет итерироваться по префиксу любого контейнера
// //Код ниже не работает, когда нужно генерировать код для константных контейнеров
// template <typename Container>
// IteratorRange<typename Container::iterator> Head(Container& c, size_t top) {
//     return IteratorRange{
//         c.begin(), next(c.begin(), std::min(top, c.size()))};
// }

// решаем проблему с помощью ключевого слова "auto"
// Здесь "auto" говорит компилятору: "возьми тип из возвращаемого значения"
template <typename Container>
auto Head(Container& c, size_t top) {
    return IteratorRange{c.begin(), next(c.begin(), std::min(top, c.size()))};
}
// ==================== END of IteratorRange template ===================

template <typename T>
size_t RangeSize(IteratorRange<T> r) {
    return distance(r.begin(), r.end());
}

// порождающая функция для IteratorRange
// порождающие функции сокращают код, перекладывая на компилятор вывод параметров шаблона
template <typename Iterator>
IteratorRange<Iterator> MakeRange(Iterator it_begin, Iterator it_end) {
    return {it_begin, it_end};
}