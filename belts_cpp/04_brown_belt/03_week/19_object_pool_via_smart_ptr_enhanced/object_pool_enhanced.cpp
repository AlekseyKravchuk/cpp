#include "test_runner.h"

#include <algorithm>
#include <memory>        // std::unique_ptr
#include <queue>
#include <set>
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
    // Определяем свой компаратор
    struct Compare {
       using is_transparent = void; // сделали компаратор прозрачным

        // Используем стандартное сравнение для ключей
        bool operator()(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) const {
            return lhs < rhs;
        }

        // Определяем функции сравнения ключа и raw-указателя
        bool operator()(const unique_ptr<T>& lhs, const T* rhs) const {
            // в С++ нельзя сравнивать указатели двух никак не связанных объектов с помощью стандартных
            // операций сравнения ("<", "<=", ">", ">=").
            return std::less<const T*>()(lhs.get(), rhs);
        }

        bool operator()(const T* lhs, const unique_ptr<T>& rhs) const {
            // Выражение less<const T*>() инстанцирует шаблон класса less для указателя const T* и создаёт объект
            // компаратора (здесь мы везде используем указатели на константу, т.к. обычные указатели могут быть неявно
            // к ним преобразованы, но не наоборот). У него будет вызван operator(), куда мы передадим два указателя.
            // Один возьмём напрямую из параметра функции, а второй вытащим из unique_ptr с помощью метода get().
            // И уже operator() вернёт нам результат сравнения.

            // Таким образом, выражение less<const T*>()(lhs.get(), rhs) вернёт нам по сути то же самое,
            // что и обычное lhs.get() < rhs, но только с гарантией, что это будет всегда правильно работать.
            return std::less<const T*>()(lhs, rhs.get());
        }
    };

    queue<unique_ptr<T>> _free;
    set<unique_ptr<T>, Compare> _allocated;  // Изменили контейнер на set
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
    auto it = _allocated.find(object);
    if (it == _allocated.end()) {
        throw invalid_argument("");
    }

    // просто так модифицировать ключ в ассоциативном контейнере нельзя - сломается механизм индексации
    // поэтому используем метод std::set::extract

//    auto node = std::move(_allocated.extract(it));
//    _free.push(std::move(node.value()));
    _free.push(std::move(_allocated.extract(it).value()));
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
