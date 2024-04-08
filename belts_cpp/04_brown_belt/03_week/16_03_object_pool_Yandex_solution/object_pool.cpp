#include "test_runner.h"

#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

int counter = 0;

struct Counted {
    Counted() { ++counter; }
    ~Counted() { --counter; }
};

template<class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* object);
    ~ObjectPool();

private:
    queue<T*> _free;
    set<T*> _allocated;
};

template<typename T>
T* ObjectPool<T>::Allocate() {
    if (_free.empty()) {
        _free.push(new T);
    }

    auto ret = _free.front();
    _free.pop();
    _allocated.insert(ret);
    return ret;
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
    if (_allocated.find(object) == _allocated.end()) {
        throw invalid_argument("");
    }
    _allocated.erase(object);
    _free.push(object);
}

template<typename T>
ObjectPool<T>::~ObjectPool() {
    for (auto x: _allocated) {
        delete x;
    }
    while (!_free.empty()) {
        auto x = _free.front();
        _free.pop();
        delete x;
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

void run() {
    ObjectPool<Counted> pool;
    const int MAX_OBJECTS_IN_POOL = 1000000;

    for (int i = 0; i < MAX_OBJECTS_IN_POOL; ++i) {
        cout << "Allocating object #" << i << endl;
        pool.Allocate();
    }


    cout << "counter from run() = " << counter << endl;
}

int main() {
    // TODO: эмулируем нехватку памяти (08:28)
    //  порядок запуска в терминале:
    // (ulimit -v 10000 && exec ./16_02_object_pool_from_scratch)
    // -v: максимальный объем виртуальной памяти, доступной процессам
    run();
    cout << "counter from main() = " << counter << endl;
//    TestRunner tr;
//    RUN_TEST(tr, TestObjectPool);
    return 0;
}
