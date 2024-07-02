// ******************************** Задание 1 ********************************
// Вы создали свой первый умный указатель ScopedPtr, который позволил сохранить объект в динамической памяти
// и автоматически контролировать время его жизни. У ScopedPtr есть и другие полезные свойства:
//    - Исключает неинициализированное состояние. Он либо пустой, либо хранит адрес существующего объекта;
//    - Запрещает копирование указателя. Тем самым исключает ситуацию, когда два указателя одновременно владеют одним объектом;
//    - Вместо неопределённого поведения при разыменовании нулевого указателя ScopedPtr выбрасывает исключение logic_error,
//      которое можно поймать и обработать приложением.

// Эти свойства указателя позволяют сделать управление объектами в динамической памяти проще. Вместо такого:
// int main() {
//     vector<Object*> objects;
//     Object* ptr = nullptr;
//     try {
//         for (int i = 0; i < 5; ++i) {
//             ptr = new Object();
//             objects.push_back(ptr);
//             // Обнуляем ptr (этот указатель уже скопирован в objects)
//             ptr = nullptr;
//         }
//     } catch (...) {
//         delete ptr;
//     }
//     // Удаляем объекты из массива указателей
//     for (Object* obj_ptr : objects) {
//         delete obj_ptr;
//     }
// }

// Можно написать лаконичнее:
// int main() {
//     vector<Object*> objects;
//     try {
//         for (int i = 0; i < 5; ++i) {
//             // Пока указатель не добавлен в контейнер objects, им владеет умный указатель
//             ScopedPtr<Object> ptr(new Object());
//             objects.push_back(ptr.GetRawPtr());

//             // Вызываем Release, чтобы указатель не удалил объект, уже
//             // вставленный в вектор.
//             ptr.Release();
//         }
//     } catch (...) {
//     }
//     for (Object* obj_ptr : objects) {
//         delete obj_ptr;
//     }
// }

// Тем не менее удалять объекты, на которые ссылаются указатели внутри вектора, всё ещё приходится вручную.
// Чтобы автоматизировать процесс, разработаем класс-обёртку PtrVector — вектор указателей.
// Он автоматически удаляет объекты в своём деструкторе. Код станет ещё проще:

// int main() {
//     PtrVector<Object> objects;
//     for (int i = 0; i < 3; ++i) {
//         ScopedPtr<Object> ptr(new Object());
//         objects.GetItems().push_back(ptr.GetRawPtr());
//         ptr.Release();
//     }

//     cout << "PtrVector copy has been destroyed"s << endl;

//     // Деструктор PtrVector автоматически удалит объекты, на которые
//     // ссылаются хранящиеся внутри него указатели
// }

// Также PtrVector будет допускать копирование:
// PtrVector<Object> objects;

// // ...

// PtrVector<Object> objects_copy(objects);

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Разработайте шаблонный класс PtrVector, оборачивающий вектор указателей.
// Необходимо сделать так, чтобы объекты, хранимые в векторе посредством указателей на них, уничтожались автоматически
// при выходе переменной-вектора из области видимости, т.е. чтобы уничтожение происходило в деструкторе
// Объекты, на которые ссылаются эти указатели, должны размещаться в динамической памяти.
// В деструкторе PtrVector должен удалить объекты по хранящимся в нём указателям.
// Данные он размещает в контейнере vector.
// Конструктор копирования PtrVector должен копировать объекты исходного вектора.
// При этом нулевые указатели исходного вектора остаются нулевыми в векторе-копии.

// Пример использования дан в заготовке решения.
// ======> ПОДСКАЗКА
// В деструкторе PtrVector примените операцию delete к каждому указателю внутри _items.
// Обработку исключений в коде копирующего конструктора можно упростить: используйте метод reserve
// и зарезервируйте в векторе место для хранения нужного количества щупалец. Тогда метод push_back не будет выбрасывать исключений.

// =========================== ЗАГОТОВКА РЕШЕНИЯ ===========================
// #include <algorithm>
// #include <cassert>
// #include <vector>

// using namespace std;

// template <typename T>
// class PtrVector {
// public:
//     PtrVector() = default;

//     // Создаёт вектор указателей на копии объектов из other
//     PtrVector(const PtrVector& other) {
//         // Реализуйте копирующий конструктор самостоятельно
//     }

//     // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
//     // в векторе _items
//     ~PtrVector() {
//         // Реализуйте тело деструктора самостоятельно
//     }

//     // Возвращает ссылку на вектор указателей
//     vector<T*>& GetItems() noexcept {
//         // Реализуйте метод самостоятельно
//     }

//     // Возвращает константную ссылку на вектор указателей
//     vector<T*> const& GetItems() const noexcept {
//         // Реализуйте метод самостоятельно
//     }

// private:
//     vector<T*> _items;
// };
// ============================================================================

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class PtrVector {
   public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из вектора "other"
    PtrVector(const PtrVector& other) {
        try {
            // Резервируем место в векторе - благодаря этому при вызове push_back() не будет выбрасываться исключение
            _items.reserve(other._items.size());

            // пробегаем в цикле по всем указателям на объекты типа "T"
            for (auto ptr : other._items) {
                // если ptr - нулевой, оставляем его нулевым
                // если ptr указывает на объект, выделяем память и инициализируем ее значением исходного (копируемого) объекта
                auto copyPtr = ptr ? new T(*ptr) : nullptr;

                // помещаем указатель-копию в вектор
                // push_back() уже не выбросит исключение, поскольку память уже зарезервирована
                _items.push_back(copyPtr);
            }
        } catch (...) {
            // удаляем объекты по хранящимся в векторе указателям (которые успели создаться)
            DeleteItems();

            // перевыбрасываем пойманное исключение
            throw;
        }
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе _items
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    vector<T*>& GetItems() noexcept {
        return _items;
    }

    // Возвращает константную ссылку на вектор указателей
    vector<T*> const& GetItems() const noexcept {
        return _items;
    }


   private:
    vector<T*> _items;

    void DeleteItems() noexcept {
        for (auto ptr : _items) {
            delete ptr;
        }
    }
};

// Эта функция main тестирует шаблон класса PtrVector
void YandexTestPtrVector() {
    // Вспомогательный "шпион", позволяющий узнать о своём удалении
    struct DeletionSpy {
        explicit DeletionSpy(bool& isDeleted) : _isDeleted(isDeleted) {}

        ~DeletionSpy() {
            _isDeleted = true;
        }

        bool& _isDeleted;
    };

    // Проверяем удаление элементов
    {
        bool spy1_is_deleted = false;
        DeletionSpy* ptr1 = new DeletionSpy(spy1_is_deleted);
        {
            PtrVector<DeletionSpy> ptr_vector;
            ptr_vector.GetItems().push_back(ptr1);
            assert(!spy1_is_deleted);

            // Константная ссылка на ptr_vector
            const auto& const_ptr_vector_ref(ptr_vector);
            // И константная, и неконстантная версия GetItems
            // должны вернуть ссылку на один и тот же вектор
            assert(&const_ptr_vector_ref.GetItems() == &ptr_vector.GetItems());
        }
        // При разрушении ptr_vector должен удалить все объекты, на которые
        // ссылаются находящиеся внутри него указателели
        assert(spy1_is_deleted);
    }

    // Вспомогательный «шпион», позволяющий узнать о своём копировании
    struct CopyingSpy {
        explicit CopyingSpy(int& copy_count)
            : copy_count_(copy_count) {
        }
        CopyingSpy(const CopyingSpy& rhs)
            : copy_count_(rhs.copy_count_)  //
        {
            ++copy_count_;
        }
        int& copy_count_;
    };

    // Проверяем копирование элементов при копировании массива указателей
    {
        // 10 элементов
        vector<int> copy_counters(10);

        PtrVector<CopyingSpy> ptr_vector;
        // Подготавливаем оригинальный массив указателей
        for (auto& counter : copy_counters) {
            ptr_vector.GetItems().push_back(new CopyingSpy(counter));
        }
        // Последний элемент содержит нулевой указатель
        ptr_vector.GetItems().push_back(nullptr);

        auto ptr_vector_copy(ptr_vector);
        // Количество элементов в копии равно количеству элементов оригинального вектора
        assert(ptr_vector_copy.GetItems().size() == ptr_vector.GetItems().size());

        // копия должна хранить указатели на новые объекты
        assert(ptr_vector_copy.GetItems() != ptr_vector.GetItems());
        // Последний элемент исходного массива и его копии - нулевой указатель
        assert(ptr_vector_copy.GetItems().back() == nullptr);
        // Проверяем, что элементы были скопированы (копирующие шпионы увеличивают счётчики копий).
        assert(all_of(copy_counters.begin(), copy_counters.end(), [](int counter) {
            return counter == 1;
        }));
    }
}

void MyTestPtrVector() {
    PtrVector<int> ptrvec;
    std::cout << "finished"s << std::endl;
}

int main() {
    // YandexTestPtrVector();
    MyTestPtrVector();

    return 0;
}