#pragma once

#include <algorithm>  // std::fill_n
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "array_ptr.h"

template <typename Type>
class SimpleVector {
   public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : _items(size), _size(size), _capacity(size) {
        std::fill_n(_items.Get(), size, 0);
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : _items(size, value), _size(size), _capacity(size) {}

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> values) : _items(values), _size(values.size()), _capacity(values.size()) {}

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return _size;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return _capacity;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return _size == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return _items[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return _items[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Referencing invalid index."s);
        } else {
            return _items[index];
        }
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Referencing invalid index."s);
        } else {
            return _items[index];
        }
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        _size = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size > _capacity) {
            size_t newCapacityVal = std::max(new_size, 2 * _capacity);

            try {
                Type* newItemsPtr = (newCapacityVal) ? new Type[newCapacityVal] : nullptr;
                std::fill_n(newItemsPtr, newCapacityVal, 0);

                // копируем имеющиеся элементы из старого месторасположения в новое
                for (size_t i = 0; i < _size; ++i) {
                    *(newItemsPtr + i) = _items[i];
                }

                // освобождаем занятую ранее память
                delete[] _items.Release();

                // перенастраиваем указатель, изменяем _size и _capacity
                _items.Set(newItemsPtr);
                _size = new_size;
                _capacity = newCapacityVal;
            } catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        } else {
            // если (new_size <= _capacity)
            // нужно рассмотреть случай, когда "new_size > _size"
            if (new_size > _size) {
                // заполняем элементы нулями посде (_size-1)-го элемента
                for(size_t i = _size; i < new_size; ++i) {
                    _items[i] = 0;
                }
            }
            _size = new_size;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return _items.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return (_items.Get() + _size);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return _items.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return (_items.Get() + _size);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return _items.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return (_items.Get() + _size);
    }

   private:
    ArrayPtr<Type> _items;  // указатель на первый элемент вектора
    size_t _size = 0;
    size_t _capacity = 0;
};