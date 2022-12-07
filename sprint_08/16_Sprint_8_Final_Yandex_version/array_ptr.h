#pragma once
#include <cassert>
#include <cstdlib>
#include <utility>
template <typename Type>
class ArrayPtr {
public:
    ArrayPtr() = default;
    explicit ArrayPtr(size_t size) : _rawPtr(size != 0 ? new Type[size] : nullptr) { }

    explicit ArrayPtr(Type* raw_ptr) noexcept : _rawPtr(raw_ptr) { }

    ArrayPtr(const ArrayPtr&) = delete;

    ArrayPtr(ArrayPtr&& other) {
        _rawPtr = std::exchange(other._rawPtr, nullptr);
    }

    ~ArrayPtr() {
        delete[] _rawPtr;
    }

    ArrayPtr& operator=(const ArrayPtr&) = delete;

    ArrayPtr& operator=(ArrayPtr&& other) {
        std::swap(_rawPtr, other._rawPtr);
        return *this;
    }
    
    Type* Release() noexcept {
        return std::exchange(_rawPtr, nullptr);
    }

    Type& operator[](size_t index) noexcept {
        return _rawPtr[index];
    }

    const Type& operator[](size_t index) const noexcept {
        return _rawPtr[index];
    }

    explicit operator bool() const {
        return _rawPtr != nullptr;
    }

    Type* Get() const noexcept {
        return _rawPtr;
    }

    void swap(ArrayPtr& other) noexcept {
        std::swap(_rawPtr, other._rawPtr);
    }

private:
    Type* _rawPtr = nullptr;
};