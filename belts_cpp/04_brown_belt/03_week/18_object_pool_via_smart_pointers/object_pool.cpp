#include "test_runner.h"

#include <algorithm>
#include <memory>        // std::unique_ptr
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>       // std::move

using namespace std;

template<class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* object);

private:
    queue<unique_ptr<T>> _free;
    unordered_map<T*, unique_ptr<T>> _allocated;

    // для того, чтобы использовать std::unique_ptr в качестве ключа ассоциативного контейнера, нам нужно знать
    // о некоторой возможности этого ассоциативного контейнера, которую мы пока не проходили
//    set<unique_ptr<T>> _allocated;
};

template<typename T>
T* ObjectPool<T>::Allocate() {
    if (_free.empty()) {
        _free.push(make_unique<T>());
    }

    // std::unique_ptr можно только перемещать - перемещаем его в локальную переменную
    unique_ptr<T> uptr = std::move(_free.front());
    _free.pop();
    T* raw_ptr = uptr.get();
    _allocated[raw_ptr] = std::move(uptr);
    return raw_ptr;
}

template<typename T>
T* ObjectPool<T>::TryAllocate() {
    if (_free.empty()) {
        return nullptr;
    }
    return Allocate();
}

template<typename T>
void ObjectPool<T>::Deallocate(T* object) {
    auto it = _allocated.find(object);
    if (it == _allocated.end()) {
        throw invalid_argument("");
    }

    _free.push(std::move(it->second));
    _allocated.erase(it);
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
