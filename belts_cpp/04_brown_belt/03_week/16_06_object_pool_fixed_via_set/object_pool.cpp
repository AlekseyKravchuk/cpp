#include "test_runner.h"

#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
//#include <unordered_map>
#include <new>     // std::bad_alloc
#include <memory>  // std::unique_ptr

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

  private:
    struct Compare {
        using is_transparent = void;  // делаем комаратор прозрачным

        // Определяем свой компаратор для std::set
        bool operator()(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) const {
            return lhs < rhs;
        }

        // Определяем функции сравнения ключа и обычного указателя:
        /*
         *  В С++ нельзя сравнивать указатели двух никак не связанных объектов с помощью стандартных операций сравнения
         *  ("<", "<=", ">", ">=").
         *  Выражение less<const T*>()(lhs.get(), rhs) вернёт нам по сути то же самое, что и обычное lhs.get() < rhs,
         *  но только с гарантией, что это будет всегда правильно работать.
         */
        bool operator()(const T* lhs, const unique_ptr<T>& rhs) const {
            return std::less<const T*>()(lhs, rhs.get());
        }

        bool operator()(const unique_ptr<T>& lhs, const T* rhs) const {
            return std::less<const T*>()(lhs.get(), rhs);
        }
    };

  private:
    queue<unique_ptr<T>> _free;
    set<unique_ptr<T>, Compare> _allocated;

};

template<typename T>
T* ObjectPool<T>::Allocate() {
    if (_free.empty()) {
        _free.push(make_unique<T>());
    }

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

    // нужно переложить объект из множества занятых объектов в очередь свободных объектов
    // росто так модифицировать ключ в ассоциативном контейнере нельзя - сломается механизм индексации
    // (в случае упорядоченного контейнера - может нарушиться порядок). А перемещение из ключа ещё как его модифицирует.
    _free.push(std::move(_allocated.extract(it).value()));
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
