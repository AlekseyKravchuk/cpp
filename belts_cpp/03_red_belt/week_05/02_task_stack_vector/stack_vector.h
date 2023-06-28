#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
   public:
    explicit StackVector(size_t a_size = 0);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    auto begin();
    auto end();
    const auto begin() const;
    const auto end() const;

    size_t Size() const;
    size_t Capacity() const;
    bool IsEmpty() const;

    void PushBack(const T& value);
    T PopBack();

   private:
    std::array<T, N> _array{};
    size_t _size{};
    size_t _capacity{};
};

template <typename T, size_t N>
auto StackVector<T, N>::begin() {
    return _array.begin();
}

template <typename T, size_t N>
auto StackVector<T, N>::end() {
    return _array.begin() + _size;
}

template <typename T, size_t N>
const auto StackVector<T, N>::begin() const {
    return _array.cbegin();
}

template <typename T, size_t N>
const auto StackVector<T, N>::end() const {
    return _array.cbegin() + _size;
}

template <typename T, size_t N>
size_t StackVector<T, N>::Capacity() const {
    return _array.max_size();
}

template <typename T, size_t N>
size_t StackVector<T, N>::Size() const {
    return _size;
}

template <typename T, size_t N>
bool StackVector<T, N>::IsEmpty() const {
    return _size == 0;
}

template <typename T, size_t N>
StackVector<T, N>::StackVector(size_t a_size) : _capacity(N) {
    if (a_size > _capacity) {
        throw std::invalid_argument("The specified size is greater than the actual capacity of the vector");
    } else {
        _size = a_size;
    }
}

template <typename T, size_t N>
void StackVector<T, N>::PushBack(const T& value) {
    if (_size == _capacity) {
        throw std::overflow_error("Vector is full, further adding elements impossible");
    } else {
        _array[_size++] = value;
    }
}

template <typename T, size_t N>
T StackVector<T, N>::PopBack() {
    if (IsEmpty()) {
        throw std::underflow_error("Vector is empty, further poping impossible");
    } else {
        return _array[--_size];
    }
}

template <typename T, size_t N>
T& StackVector<T, N>::operator[](size_t index) {
    return _array[index];
}

template <typename T, size_t N>
const T& StackVector<T, N>::operator[](size_t index) const {
    return _array[index];
}