#include "test_runner.h"

#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <memory>  // std::unique_ptr

using namespace std;

// этот счетчик показывает, сколько объектов существует в программе на данный момент
int counter = 0;

struct Counted {
    Counted() { ++counter; }
    ~Counted() { --counter; }
};

template<typename T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* object);

private:
    queue<std::unique_ptr<T>> free;       // выделенные объекты
    unordered_map<T*, unique_ptr<T>>    allocated;  // освобожденные объекты
};

template<typename T>
T* ObjectPool<T>::Allocate() {
    if (free.empty()) {
        free.push(make_unique<T>());
    }
    auto uptr = std::move(free.front());
    free.pop();

    T* raw_ptr = uptr.get();
    allocated[raw_ptr] = std::move(uptr);

    return raw_ptr;
}

template<typename T>
T* ObjectPool<T>::TryAllocate() {
    if (free.empty()) {
        return nullptr;
    }
    return Allocate();
}

template<typename T>
void ObjectPool<T>::Deallocate(T* object) {
    auto it = allocated.find(object);
    if (it == allocated.end()) {
        throw invalid_argument("");
    }
    free.push(std::move(it->second));
    allocated.erase(it);

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

    cout << "Counter before loop = " << counter << endl;

    // будем выделять объекты в цикле
    for (int i = 0; i < 1000000; ++i) {
        cout << "Allocating object #" << i << endl;
        pool.Allocate();
    }

    cout << "Counter AFTER loop = " << counter << endl;
}

int main() {
//    TestRunner tr;
//    RUN_TEST(tr, TestObjectPool);

    // когда мы выходим из функции, все её локальные объекты уничтожаются
    run();

    // перед выходом из main не должно остаться никаких объектов в пуле
    cout << "Counter before EXIT = " << counter << endl;

    return 0;
}
