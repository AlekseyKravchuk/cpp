#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>

#include "test_runner.h"
using namespace std;

template <typename T>
class ObjectPool {
   public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* object_ptr);
    ~ObjectPool();

   private:
    std::set<T*> _allocated;   // выделенные объекты
    std::queue<T*> _released;  // освобожденные объекты
};

template <typename T>
T* ObjectPool<T>::Allocate() {
    if (!_released.empty()) {
        T* obj_ptr = _released.front();  // копируем значение указателя из НАЧАЛА очереди в переменную "obj_ptr"
        _allocated.insert(obj_ptr);      // вставляем указатель в множество "_allocated"
        _released.pop();                 // удаляем элемент из НАЧАЛА очереди

        return obj_ptr;
    } else {
        T* new_obj_ptr = new T;
        _allocated.insert(new_obj_ptr);

        return new_obj_ptr;
    }
}

template <typename T>
T* ObjectPool<T>::TryAllocate() {
    return _released.empty() ? nullptr : Allocate();
}

template <typename T>
void ObjectPool<T>::Deallocate(T* object_ptr) {
    if (auto it = _allocated.find(object_ptr); it != _allocated.end()) {
        _released.push(*it);
        _allocated.erase(it);
    } else {
        throw std::invalid_argument("Pool of allocated objects does not contain given object");
    }
}

template <typename T>
ObjectPool<T>::~ObjectPool() {
    for (T* obj_ptr : _allocated) {
        delete obj_ptr;
    }
    _allocated.clear();

    // // Removing from a set while iterating it (The standard associative-container ERASE IDIOM)
    // for (auto it = _allocated.begin(); it != _allocated.end();) {
    //     delete *it;
    //     it = _allocated.erase(it);
    // }

    while (!_released.empty()) {
        T* obj_ptr = _released.front();
        delete obj_ptr;
        _released.pop();
    }
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
