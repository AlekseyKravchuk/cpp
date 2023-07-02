#pragma once

#include <array>
#include <cstddef>  // size_t
#include <utility>  // std::pair

// TAirport should be enum with sequential items and last item TAirport::Last_
template <typename TAirport>
class AirportCounter {
   public:
    // конструктор по умолчанию: список элементов пока пуст
    AirportCounter() = default;

    // конструктор от диапазона элементов типа TAirport
    template <typename TIterator>
    AirportCounter(TIterator begin, TIterator end) {
        while (begin != end) {
            ++_departures[static_cast<size_t>(*begin++)];
        }
    }

    // // получить количество элементов, равных данному
    size_t Get(TAirport airport) const {
        return _departures[static_cast<size_t>(airport)];
    }

    // // добавить данный элемент
    // void Insert(TAirport airport);

    // // удалить одно вхождение данного элемента
    // void EraseOne(TAirport airport);

    // // удалить все вхождения данного элемента
    // void EraseAll(TAirport airport);

    using Item = std::pair<TAirport, size_t>;
    using Items = std::array<Item, static_cast<size_t>(TAirport::Last_)>;

    // // получить некоторый объект, по которому можно проитерироваться, получив набор объектов
    // // типа Item - пар (аэропорт, количество), упорядоченных по аэропорту
    Items GetItems() const {
    }

   private:
    // std::array<TAirport, TAirport::Last_> _departures;
    std::array<size_t, static_cast<size_t>(TAirport::Last_)> _departures{};
};
