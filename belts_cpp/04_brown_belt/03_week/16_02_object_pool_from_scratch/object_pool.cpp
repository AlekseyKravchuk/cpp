#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>
#include <set>
#include <deque>

using namespace std;

template<typename T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* obj);
    ~ObjectPool();

private:
    set<T*>   _allocated;
    deque<T*> _released;
};

template<typename T>
T* ObjectPool<T>::Allocate() {
    // если есть хотя бы один освобождённый объект, то его надо перенести в множество выделенных
    // и вернуть указатель на него в качестве результата функции
    if (!_released.empty()) {
        T* obj_ptr = _released.front();
        _allocated.insert(obj_ptr);
        _released.pop_front();

        return obj_ptr;
    } else {
        // если же освобождённых объектов нет, то создаётся новый объект, помещается в множество выделенных,
        // и указатель на него возвращается в качестве результата функции
        auto [it, inserted] = _allocated.insert(new T());
        return inserted ? *it : nullptr;
    }
}

template<typename T>
T* ObjectPool<T>::TryAllocate() {
    if (!_released.empty()) {
        T* obj_ptr = _released.front();
        _allocated.insert(obj_ptr);
        _released.pop_front();

        return obj_ptr;
    } else {
        return nullptr;
    }
}

template<typename T>
void ObjectPool<T>::Deallocate(T* obj_ptr) {
    if (auto it = _allocated.find(obj_ptr); it != _allocated.end()) {
        _released.push_back(*it);
        _allocated.erase(it);
    } else {
        throw std::invalid_argument("Can not deallocate object - it is not present in ALLOCATED.");
    }
}

template<typename T>
ObjectPool<T>::~ObjectPool() {
    for (auto p : _allocated) {
        delete p;
    }
    _allocated.clear();

    for (auto p : _released) {
        delete p;
    }
    _released.clear();
}

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
