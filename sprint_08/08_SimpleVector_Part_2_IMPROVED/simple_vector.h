#pragma once

#include <algorithm>  // std::fill_n
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <iterator>  // std::prev
#include <stdexcept>

#include "array_ptr.h"

using namespace std::literals;

// template <typename Iterator>
// void PrintRange(Iterator range_begin,
//                 Iterator range_end) {
//     for (auto it = range_begin; it != range_end; ++it) {
//         if (std::next(it) != range_end) {
//             std::cout << *it << ", "s;
//         } else {
//             std::cout << *it << std::endl;
//         }
//     }
// }

template <typename Type>
class SimpleVector {
   public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : _arrayPtr(size), _size(size), _capacity(size) {
        std::fill_n(_arrayPtr.Get(), size, 0);
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : _arrayPtr(size), _size(size), _capacity(size) {
        std::fill_n(_arrayPtr.Get(), size, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> values) : _arrayPtr(values.size()), _size(values.size()), _capacity(values.size()) {
        Type* destBeginPtr = _arrayPtr.Get();
        std::copy(values.begin(), values.end(), destBeginPtr);
    }

    // конструктор копирования, реализованный согласно идиоме copy-and-swap
    // 1) Создать внутри конструктора копирования временный массив и последовательно скопировать внутрь него элементы массива "other".
    //    Если на этом этапе будет выброшено исключение, деструктор временного массива освободит память от его элементов.
    // 2) Когда временный массив будет содержать копию исходного массива, останется использовать метод swap и обменять состояние текущего экземпляра класса и временного массива.
    SimpleVector(const SimpleVector& other) {
        // Сначала надо удостовериться, что текущий массив пуст
        assert(_size == 0 && _arrayPtr.Get() == nullptr);

        // создаем временный массив такого же, как и "other" размера
        SimpleVector<Type> tmpVec(other.GetSize());

        // копируем во временный массив элементы из "other"
        std::copy(other.begin(), other.end(), tmpVec.begin());

        // После того как элементы скопированы, обмениваем все данные текущего массива и tmpVec
        swap(tmpVec);
        // Теперь tmp пуст, а текущий список содержит копию элементов other
    }

    // оператор присваивания
    SimpleVector& operator=(const SimpleVector& rhs) {
        // 1) Проверяем, не выполняется ли присваивание массива самому себе, сравниваем адреса левого и правого аргументов операции присваивания.
        assert(_arrayPtr.Get() != rhs._arrayPtr.Get());

        // 2) создаем временный массив такого же размера, как и "rhs"
        // Выбрасывание исключения на этом этапе никак не повлияет на состояние текущего объекта (левый аргумент операции присваивания).
        SimpleVector<Type> tmpVec(rhs.GetSize());

        // копируем в него элементы из rhs
        std::copy(rhs.begin(), rhs.end(), tmpVec.begin());

        // 3) Обмениваем содержимое временной копии и текущего объекта. Операция swap исключений не выбрасывает и выполняется быстро.
        //    Временная копия будет содержать предыдущее значение левого аргумента, а текущий экземпляр — копию правого аргумента операции присваивания.
        swap(tmpVec);
        // При выходе из операции = временный объект будет разрушен, освободив память от предыдущего содержимого списка.

        return *this;
    }

    void PushBack(const Type& value) {
        if (_capacity == 0) {
            *this = SimpleVector(1, value);
            return;
        }

        // если вектор заполнен полностью
        if (_size >= _capacity) {
            // создаем новый массив удвоенного размера
            SimpleVector<Type> tmpVec(GetSize() * 2);

            // копируем в него все элементы из текущего массива
            std::copy(begin(), end(), tmpVec.begin());

            // и устанавливаем "_size" нового вектора равным "this->_size"
            tmpVec._size = _size;

            // меняем содержимое временного и текущего массивов
            swap(tmpVec);
            // При выходе из метода "SimpleVector::PushBack" временный массив, содержащий "начинку" исходного массива, будет разрушен
        }

        _arrayPtr[_size] = value;
        ++_size;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        --_size;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        // для вектора вместимостью 0 "_capacity" должна стать равной 1
        if (_capacity == 0) {
            *this = SimpleVector(1, value);
            return _arrayPtr.Get();
        }

        // поскольку в результате итератор "pos" может быть инвалидирован, сохраняем нужную нам информацию
        size_t indexToInsert = pos - begin();

        // если вектор заполнен полностью, создаем новый вектор удвоенного размера и копируем туда все элементы
        if (_size >= _capacity) {
            // создаем новый массив удвоенного размера
            SimpleVector<Type> tmpVec(_size * 2);

            // копируем в него все элементы из текущего массива
            std::copy(begin(), end(), tmpVec.begin());

            // и устанавливаем "_size" нового вектора равным "this->_size"
            tmpVec._size = _size;

            // меняем содержимое временного и текущего массивов
            swap(tmpVec);
        }

        auto first = begin() + indexToInsert;
        auto last = end();
        auto d_last = last + 1;

        // передвигаем все элементы начиная итератора "it" на 1 позицию вправо
        std::copy_backward(first, last, d_last);

        // вставляем элемент в нужную позицию
        *first = value;
        ++_size;

        return first;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        auto it = const_cast<Iterator>(pos);

        std::copy(std::next(it), end(), it);
        --_size;

        return it;
    }

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
        return _arrayPtr[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return _arrayPtr[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Referencing invalid index."s);
        } else {
            return _arrayPtr[index];
        }
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Referencing invalid index."s);
        } else {
            return _arrayPtr[index];
        }
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        _size = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {

        // Случай 1: новый размер меньше или равен текущему
        if (new_size <= _size) {
            _size = new_size;
            return;
        }

        // Случай 2: новый размер превышает допустимую вместимость текущего вектора
        if (new_size > _capacity) {
            // создаем временный объект SimpleVector подходящей размерности
            SimpleVector<Type> tmpVec(std::max(new_size, 2 * _capacity));

            // копируем элементы во временный массив
            std::copy(begin(), end(), tmpVec.begin());

            // меняем содержимое текущего вектора (*this) и временного "tmpVec"
            swap(tmpVec);
            // после выхода из блока "if (...)" временный вектор с содержимым от исходного будет корректно уничтожен
        } else {
            // при увеличении размера массива новый размер вектора не превышает его вместимость
            // остается всего лишь заполнить добавленные элементы значением по умолчанию для типа Type
            std::fill(end(), end() + (_capacity - _size), Type());

            // после чего можно изменить размер
            _size = new_size;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return _arrayPtr.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return (_arrayPtr.Get() + _size);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return _arrayPtr.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return (_arrayPtr.Get() + _size);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return _arrayPtr.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return (_arrayPtr.Get() + _size);
    }

    // Чтобы обменять содержимое векторов, достаточно обменять значения их полей.
    void swap(SimpleVector& other) noexcept {
        std::swap(_capacity, other._capacity);
        std::swap(_size, other._size);
        _arrayPtr.swap(other._arrayPtr);
    }

   private:
    ArrayPtr<Type> _arrayPtr;  // умный указатель на первый элемент вектора, хранящего элементы типа "Type"
    size_t _size = 0;
    size_t _capacity = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        [](const Type& left, const Type& right) {
                                            return left > right;
                                        });
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs > rhs) || (lhs == rhs);
}