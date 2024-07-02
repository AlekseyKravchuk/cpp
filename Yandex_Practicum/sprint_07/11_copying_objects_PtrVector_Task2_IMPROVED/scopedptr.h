#pragma once

#include <stdexcept>  // содержит std::logic_error

template <typename T>
class ScopedPtr {
   public:
    ScopedPtr() = default;

    explicit ScopedPtr(T* rawPtr) noexcept : _ptr(rawPtr) { }

    // Запрещаем копирование указателя
    ScopedPtr(const ScopedPtr&) = delete;

    ~ScopedPtr() {
        delete _ptr;
    }

    T* GetRawPtr() const noexcept {
        return _ptr;
    }

    T* Release() noexcept {
        T* p = _ptr;
        _ptr = nullptr;
        return p;
    }

    explicit operator bool() const {
        return _ptr != nullptr;
    }

    T* operator->() const {
        using namespace std::literals;
        if (!_ptr) {
            throw std::logic_error("Scoped ptr is null"s);
        }
        return _ptr;
    }

    T& operator*() const {
        using namespace std::literals;
        if (!_ptr) {
            throw std::logic_error("Scoped ptr is null"s);
        }
        return *_ptr;
    }

   private:
    T* _ptr = nullptr;
};