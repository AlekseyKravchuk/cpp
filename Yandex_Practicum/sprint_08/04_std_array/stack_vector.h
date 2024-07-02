#pragma once

#include <array>
#include <stdexcept>

using namespace std;

template <typename T, size_t N>
class StackVector {
   public:
    explicit StackVector(size_t a_size = 0) : _size(a_size) {
        if (Capacity() < _size) {
            throw std::invalid_argument("desired size of StackVector is greater than its capacity"s);
        }
    }

    T& operator[](size_t index) {
        return _arr.at(index);
    }

    const T& operator[](size_t index) const {
        return _arr.at(index);
    }

    auto begin() {
        return _arr.begin();
    }

    auto begin() const {
        return _arr.cbegin();
    }

    auto end() {
        return _arr.begin() + _size;
    }

    auto end() const {
        return _arr.cbegin() + _size;
    }

    size_t Size() const {
        return _size;
    }

    size_t Capacity() const {
        return _arr.max_size();
    }

    void PushBack(const T& value) {
        if (_size == Capacity()) {
            throw std::overflow_error("StackVector memory is exhausted."s);
        } else {
            _arr[_size] = value;
            ++_size;
        }

    }

    T PopBack() {
        if (!_size) {
            throw std::underflow_error("StackVector is empty."s);
        }

        return _arr[(_size--) - 1];
    }

   private:
    size_t _size = 0;
    std::array<T, N> _arr;
};
