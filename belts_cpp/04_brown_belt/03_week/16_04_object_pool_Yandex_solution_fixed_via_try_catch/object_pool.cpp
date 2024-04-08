#include "test_runner.h"

#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <new>  // std::bad_alloc

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
    /*
     * ПРИМЕЧАНИЕ
     * std::set - это дерево, т.е. динамическая структура. Отдельные элементы дерева также выделяются в куче.
     * Т.е. для того, чтобы добавить указатель в это множество, нам нужно создать под этот указатель элемент в куче.
     * И в этот момент нам может не хватить памяти -> бросается bad_alloc
     */
};

template<typename T>
T* ObjectPool<T>::Allocate() {
    if (_free.empty()) {
        _free.push(new T);  // <- 1-ое место, где может быть выброшено исключение из-за нехватки памяти
                            // при этом утечки памяти не будет
    }

    // === перемещаем указатель из очереди в множество ===
    // сохраняем указатель в локальном объекте
    T* ptr = _free.front();

    // удаляем указатель с вершины очереди
    _free.pop();

    // помещаем локальный указатель в множество занятых объектов
    _allocated.insert(ptr);  // 2-ое место, где может быть выброшено исключение из-за нехватки памяти
                             // здесь произойдет УТЕЧКА ПАМЯТИ из-за того, что локальная переменная "ptr" (указатель)
                             // будет удалена и выделенный в куче элемент типа "T" окажется недоступен извне
    // ===================================================

    // передаем указатель на вызывающую сторону
    // при этом этот же указатель содержится в множестве занятых объектов (_allocated)
    return ptr;
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
//  const int MAX_OBJECTS_IN_POOL = 1000000;
    const int MAX_OBJECTS_IN_POOL = 10000191;

    cout << "counter BEFORE loop = " << counter << endl;

    // Хотим отловить исключение std::bad_alloc и как-то на него отреагировать
    try {
        for (int i = 0; i < MAX_OBJECTS_IN_POOL; ++i) {
            cout << "Allocating object #" << i << endl;
            pool.Allocate();
        }
    } catch (const std::bad_alloc& e) {
        cout << e.what() << endl;
    }

    cout << "counter AFTER loop = " << counter << endl;
}

int main() {
    // эмулируем нехватку памяти c помощью утилиты ulimit:
    // (ulimit -v 10000 && exec ./16_04_object_pool_Yandex_solution_fixed_via_try_catch)
    // -v: максимальный объем виртуальной памяти, доступной процессам
    run();
    cout << "counter BEFORE EXIT = " << counter << endl;
//    TestRunner tr;
//    RUN_TEST(tr, TestObjectPool);

// Результат работы прогрыммы с искусственным ограничением памяти (налицо утечка памяти):
/* (ulimit -v 100009 && exec ./16_04_object_pool_Yandex_solution_fixed_via_try_catch)
 * ...
 * Allocating object #1202050
 * std::bad_alloc
 * counter AFTER loop = 1202051
 * counter BEFORE EXIT = 1  // !!!! <= утечка памяти
 */
    return 0;
}
