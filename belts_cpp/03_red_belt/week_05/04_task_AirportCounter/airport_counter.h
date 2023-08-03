#pragma once

#include <array>
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

    // // получить количество элементов, равных данному; "TAirport" - это enum
    size_t Get(TAirport airport) const {
        return _departures[static_cast<size_t>(airport)];
    }

    // добавить данный элемент
    void Insert(TAirport airport) {
        ++_departures[static_cast<size_t>(airport)];
    }

    // удалить одно вхождение данного элемента
    void EraseOne(TAirport airport) {
        if (_departures[static_cast<size_t>(airport)]) {
            --_departures[static_cast<size_t>(airport)];
        }
    }

    // удалить все вхождения данного элемента
    void EraseAll(TAirport airport) {
        _departures[static_cast<size_t>(airport)] = 0;
    }

    using Item = std::pair<TAirport, size_t>;
    using Items = std::array<Item, static_cast<size_t>(TAirport::Last_)>;

    // // получить некоторый объект, по которому можно проитерироваться, получив набор объектов
    // // типа Item - пар (аэропорт, количество), упорядоченных по аэропорту
    Items GetItems() const {
        Items result;
        for (size_t i = 0; i < static_cast<size_t>(TAirport::Last_); ++i) {
            result[i] = {static_cast<TAirport>(i), _departures[i]};
        }

        return result;
    }

   private:
    std::array<size_t, static_cast<size_t>(TAirport::Last_)> _departures{};
};