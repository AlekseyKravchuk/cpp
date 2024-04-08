#include "test_runner.h"

#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
//#include <set>
#include <unordered_map>
#include <new>     // std::bad_alloc
#include <memory>  // std::unique_ptr

using namespace std;

/*
 * Исправляем утечку памяти без использования try/catch - через smart pointers.
 * Проблемы начались из-за того, что мы в явном виде вызываем оператор "delete".
 */

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

private:
    // избавляемся от сырых указателей
    // queue<T*> _free;
    // set<T*> _allocated;

    queue<unique_ptr<T>> _free;

    // для того, чтобы использовать std::unique_ptr в качестве ключа ассоциативного контейнера,
    // нужно знать о некоторой особенности этого ассоциативного контейнера
    // set<T*> _allocated;

    // делаем "костыль"
    unordered_map<T*, unique_ptr<T>> _allocated;

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
        _free.push(make_unique<T>());  // <- 1-ое место, где может быть выброшено исключение из-за нехватки памяти
                            // при этом утечки памяти не будет
    }

    // === перемещаем указатель из вершины очереди в множество ===
    // перемещаем "unique_ptr" из очереди в локальный "unique_ptr"
    unique_ptr<T> uptr = std::move(_free.front());

    // удаляем указатель с вершины очереди
    _free.pop();

    // помещаем локальный указатель в множество занятых объектов
    // _allocated.insert({uptr.get(), std::move(uptr)});  // 2-ое место, где может быть выброшено исключение из-за нехватки
    //                         // памяти здесь произойдет УТЕЧКА ПАМЯТИ из-за того, что локальная переменная "uptr"
    //                         // будет удалена и выделенный в куче элемент типа "T" окажется недоступен извне

    T* raw_ptr = uptr.get();
    // _allocated.insert({raw_ptr, std::move(uptr)});
    _allocated[raw_ptr] = std::move(uptr);

    // ===================================================

    // передаем указатель на вызывающую сторону
    // при этом этот же указатель содержится в множестве занятых объектов (_allocated)
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

    // нужно переложить объект из хеш-таблицы занятых объектов в очередь свободных объектов
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

void run() {
    ObjectPool<Counted> pool;
//  const int MAX_OBJECTS_IN_POOL = 1000000;
    const int MAX_OBJECTS_IN_POOL = 10000191;

    cout << "counter BEFORE loop = " << counter << endl;

    // поскольку функция Deallocate шаблонная, нам нужно убедиться, что она правильно компилируется:
    // pool.Deallocate(nullptr);

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
