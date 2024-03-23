#include "test_runner.h"

#include <algorithm>
#include <memory>        // std::unique_ptr
#include <queue>
#include <unordered_set>
//#include <set>
#include <stdexcept>
#include <string>
#include <utility>       // std::move
#include <functional>    // std::less

using namespace std;

template<class T>
class ObjectPool {
public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* object);

private:
    queue<unique_ptr<T>> _free;
    unordered_set<unique_ptr<T>> _allocated;  // Изменили контейнер на set
                                             // и указали прозрачный компаратор вторым аргументом
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
    _allocated.insert(std::move(uptr));
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
    unique_ptr<T> uptr = unique_ptr<T>(object);
    auto it = _allocated.find(uptr);
    if (it == _allocated.end()) {
        throw invalid_argument("");
    }

    // просто так модифицировать ключ в ассоциативном контейнере нельзя - сломается механизм индексации
    // поэтому используем метод std::set::extract

//    auto node = std::move(_allocated.extract(it));
//    _free.push(std::move(node.value()));
    _free.push(std::move(_allocated.extract(it).value()));

    // Если не сделать "uptr.release()", то "uptr" будет удалён при выходе из функции,
    // поскольку unique_ptr удалит объект, которым владеет!
    uptr.release();  // Отзываем владение
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
