#include "test_runner.h"

#include <cstddef>    // нужно для nullptr_t
#include <utility>    // std::exchange
#include <algorithm>  // std::swap

using namespace std;

// Реализуйте шаблон класса UniquePtr
template<typename T>
class UniquePtr {
  public:
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr() = default;
    explicit UniquePtr(T* ptr);
    UniquePtr(UniquePtr&& other);
    UniquePtr& operator=(UniquePtr&& other);
    UniquePtr& operator=(nullptr_t);
    ~UniquePtr();

    T& operator*() const;
    T* operator->() const;
    T* Release();
    void Reset(T* ptr);
    void Swap(UniquePtr& other);
    T* Get() const;

  private:
    T* _raw_ptr{nullptr};
};
// =======================================================
template<typename T>
UniquePtr<T>::UniquePtr(T* ptr) : _raw_ptr(ptr) { }

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) {
    _raw_ptr = other._raw_ptr;
    other._raw_ptr = nullptr;
}

template<typename T>
T* UniquePtr<T>::Release() {
    T* tmp = _raw_ptr;
    _raw_ptr = nullptr;
    return tmp;
//    return std::exchange(_raw_ptr, nullptr);
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) {
    if (_raw_ptr != other._raw_ptr) {
        delete _raw_ptr;
        _raw_ptr = std::exchange(other._raw_ptr, nullptr);
    }

    return *this;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(nullptr_t) {
    delete _raw_ptr;
    _raw_ptr = nullptr;
    return *this;
}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    delete _raw_ptr;
}

template<typename T>
T& UniquePtr<T>::operator*() const {
    return *_raw_ptr;
}

template<typename T>
T* UniquePtr<T>::operator->() const {
    return _raw_ptr;
}

template<typename T>
void UniquePtr<T>::Reset(T* ptr) {
    delete _raw_ptr;
    _raw_ptr = std::exchange(ptr, nullptr);
}

template<typename T>
void UniquePtr<T>::Swap(UniquePtr& other) {
    std::swap(_raw_ptr, other._raw_ptr);
}

template<typename T>
T* UniquePtr<T>::Get() const {
    return _raw_ptr;
}
// =========================================================

struct Item {
    static int counter;
    int value;

    Item(int v = 0) : value(v) {
        ++counter;
    }

    Item(const Item& other) : value(other.value) {
        ++counter;
    }

    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
}
