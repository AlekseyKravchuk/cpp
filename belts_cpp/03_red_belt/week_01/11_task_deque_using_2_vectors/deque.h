#pragma once

#include <stdexcept>
#include <vector>

using namespace std::literals;

template <typename T>
class Deque {
   public:
    Deque() = default;

    bool Empty() const noexcept {
        return _front.empty() && _back.empty();
    }

    size_t Size() const noexcept {
        return _front.size() + _back.size();
    }

    T& operator[](size_t index) noexcept {
        if (index < _front.size()) {
            auto front_index = _front.size() - index - 1;
            return _front[front_index];
        } else {
            auto back_index = index - _front.size();
            return _back[back_index];
        }
    }

    const T& operator[](size_t index) const noexcept {
        if (index < _front.size()) {
            auto front_index = _front.size() - index - 1;
            return _front[front_index];
        } else {
            auto back_index = index - _front.size();
            return _back[back_index];
        }
    }

    T& At(size_t index) {
        if (index < Size()) {
            return this->operator[](index);
        } else {
            throw std::out_of_range("Deque index is out of range"s);
        }
    }

    const T& At(size_t index) const {
        if (index < Size()) {
            return this->operator[](index);
        } else {
            throw std::out_of_range("Deque index is out of range"s);
        }
    }

    T& Front() {
        return !Empty() ? (!_front.empty() ? _front.back() : _back.front())
                        : throw std::runtime_error("Deque is empty"s);
    }

    const T& Front() const {
        return !Empty() ? (!_front.empty() ? _front.back() : _back.front())
                        : throw std::runtime_error("Deque is empty"s);
    }

    T& Back() {
        return !Empty() ? (!_back.empty() ? _back.back() : _front.front())
                        : throw std::runtime_error("Deque is empty"s);
    }

    const T& Back() const {
        return !Empty() ? (!_back.empty() ? _back.back() : _front.front())
                        : throw std::runtime_error("Deque is empty"s);
    }

    void PushFront(const T& value) {
        _front.push_back(value);
    }

    void PushBack(const T& value) {
        _back.push_back(value);
    }

   private:
    std::vector<T> _front{};
    std::vector<T> _back{};
};
