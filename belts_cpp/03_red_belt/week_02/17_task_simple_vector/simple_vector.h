#pragma once

#include <algorithm>
#include <cstddef>  // size_t

template <typename T>
class SimpleVector {
   public:
    SimpleVector() = default;
    explicit SimpleVector(size_t size);
    SimpleVector(std::initializer_list<T> values);
    ~SimpleVector();

    T& operator[](size_t index);

    // ======== Пара неконстантных методов "begin" и "end" ========
    T* begin();
    T* end();
    // ============================================================

    // ======== Пара КОНСТАНТНЫХ методов "begin" и "end" (возвращают указатель на КОНСТАНТНОЕ значение) ========
    const T* begin() const;
    const T* end() const;
    // =========================================================================================================

    size_t Size() const;
    size_t Capacity() const;
    void PushBack(const T& value);

   private:
    size_t _size{};
    size_t _capacity{};
    T* _data{nullptr};
};

template <typename T>
SimpleVector<T>::SimpleVector(size_t size)
    : _size(size),
      _capacity(size),
      _data(new T[size]) {}

template <typename T>
SimpleVector<T>::SimpleVector(std::initializer_list<T> values)
    : SimpleVector<T>::SimpleVector(values.size()) {
    std::copy(std::begin(values), std::end(values), _data);
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
    delete[] _data;
}

// оператор доступа к элементу вектора по индексу (оператор индексирования, subscript operator / array index operator)
template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
    return _data[index];  // эквивалентно "return *(_data + index);"
}

// ======== Пара неконстантных методов "begin" и "end" ========
template <typename T>
T* SimpleVector<T>::begin() { return _data; }

template <typename T>
T* SimpleVector<T>::end() { return _data + _size; }  // в качестве возвр. значения используем УКАЗАТЕЛЬ на one-past-the-end
// ============================================================

// ======== Пара КОНСТАНТНЫХ методов "begin" и "end" (возвращают указатель на КОНСТАНТНОЕ значение) ========
template <typename T>
const T* SimpleVector<T>::begin() const { return _data; }

template <typename T>
const T* SimpleVector<T>::end() const { return _data + _size; }
// =========================================================================================================

template <typename T>
size_t SimpleVector<T>::Size() const { return _size; }

template <typename T>
size_t SimpleVector<T>::Capacity() const { return _capacity; }

template <typename T>
void SimpleVector<T>::PushBack(const T& value) {
    if (_size >= _capacity) {
        size_t new_cap = (_capacity == 0) ? 1 : (2 * _capacity);
        T* ptr_newly_allocated = new T[new_cap];
        std::copy(begin(), end(), ptr_newly_allocated);
        std::swap(_data, ptr_newly_allocated);
        delete[] ptr_newly_allocated;
        _capacity = new_cap;
    }

    _data[_size++] = value;
}
