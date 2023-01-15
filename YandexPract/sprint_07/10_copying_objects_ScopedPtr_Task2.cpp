// ******************************** Задание 2 ********************************
// Вы сделали класс ScopedPtr умным. Осталось превратить его в указатель.
// Переопределите операции разыменования * и доступа к полям класса ->. При попытке вызвать их у нулевого указателя эти операции должны выбрасывать исключение std::logic_error, а не приводить к неопределённому поведению. Всё-таки это умный указатель. Перегруженная операция приведения к типу bool облегчает проверку на nullptr.
// В заготовке кода — ваше решение предыдущей задачи. На его основе реализуйте недостающие операции.
// Сигнатура новых операций умного указателя:

// =========================== ЗАГОТОВКА ===========================
// // Умный указатель, удаляющий связанный объект при своём разрушении.
// // Параметр шаблона T задаёт тип объекта, на который ссылается указатель
// template <typename T>
// class ScopedPtr {
// public:
//     //...

//     // Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
//     // на какой-либо объект
//     explicit operator bool() const noexcept {
//         // Реализуйте самостоятельно
//     }

//     // Оператор разыменования возвращает ссылку на объект
//     // Выбрасывает исключение std::logic_error, если указатель нулевой
//     T& operator*() const {
//         // Реализуйте самостоятельно
//     }

//     // Оператор -> должен возвращать указатель на объект
//     // Выбрасывает исключение std::logic_error, если указатель нулевой
//     T* operator->() const {
//         // Реализуйте самостоятельно
//     }

//     // ...

// private:
//     T* ptr_ = nullptr;
// };
// ==================================================================

#include <cassert>
#include <iostream>
#include <stdexcept>  // std::logic_error
#include <string>
#include <utility>  // std::exchange

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
   public:
    // Конструктор по умолчанию создаёт нулевой указатель,
    // так как поле "_ptr" имеет значение по умолчанию "nullptr"
    ScopedPtr() = default;

    // Создаёт указатель, ссылающийся на переданный "rawPtr".
    // "rawPtr" ссылается либо на объект, созданный в куче при помощи new,
    // либо является нулевым указателем
    // Спецификатор noexcept обозначает, что метод не бросает исключений
    explicit ScopedPtr(T* rawPtr) noexcept {
        _ptr = rawPtr;
    }

    // Удаляем у класса конструктор копирования
    ScopedPtr(const ScopedPtr&) = delete;

    // Деструктор. Удаляет объект, на который ссылается умный указатель.
    ~ScopedPtr() {
        // Операции "delete" можно безопасно передавать указатель, равный "nullptr", — в этом случае она ничего не делает.
        delete _ptr;
    }

    // Возвращает указатель, хранящийся внутри ScopedPtr
    T* GetRawPtr() const noexcept {
        return _ptr;
    }

    // Прекращает владение объектом, на который ссылается умный указатель.
    // Возвращает прежнее значение "сырого" указателя и устанавливает поле "_ptr" в null
    T* Release() noexcept {
        return std::exchange(_ptr, nullptr);
    }

    // Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
    // на какой-либо объект
    explicit operator bool() const noexcept {
        return _ptr != nullptr;
    }

    // Оператор разыменования возвращает ссылку на объект
    // Выбрасывает исключение "std::logic_error", если указатель нулевой
    T& operator*() const {
        using namespace std::literals;

        if (_ptr) {
            return *_ptr;
        } else {
            throw std::logic_error("Attempt to dereference nullptr"s);
        }
    }

    // Оператор -> должен возвращать указатель на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T* operator->() const {
        using namespace std::literals;

        if (_ptr) {
            return _ptr;
        } else {
            throw std::logic_error("Attempt to dereference nullptr"s);
        }
    }

   private:
    T* _ptr = nullptr;
};

// Этот main тестирует класс ScopedPtr
int main() {
    // Проверка работы оператора приведения к типу bool
    {
        // Для нулевого указателя приведение к типу bool возвращает false
        const ScopedPtr<int> empty_ptr;
        assert(!empty_ptr);

        // Для ненулевого указателя приведение к типу bool возвращает true
        const ScopedPtr<int> ptr_to_existing_object(new int(0));
        assert(ptr_to_existing_object);

        static_assert(noexcept(static_cast<bool>(ptr_to_existing_object)));
    }

    // Проверка работы оператора разыменования *
    {
        std::string* raw_ptr = new std::string("hello");
        ScopedPtr<std::string> smart_ptr(raw_ptr);
        // Ссылка, возвращаемая оператором разыменования, должна ссылаться на объект,
        // на который указывает умный указатель
        assert(&*smart_ptr == raw_ptr);

        try {
            ScopedPtr<int> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            *empty_ptr;
            // Сюда попасть мы не должны
            assert(false);
        } catch (const std::logic_error&) {
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Проверка работы оператора ->
    {
        std::string* raw_ptr = new std::string("hello");
        ScopedPtr<std::string> smart_ptr(raw_ptr);
        // Доступ к членам класса через умный указатель должен быть аналогичен
        // доступу через "сырой" указатель
        assert(smart_ptr->data() == raw_ptr->data());

        try {
            ScopedPtr<std::string> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            empty_ptr->clear();
            // Сюда попасть мы не должны
            assert(false);
        } catch (const std::logic_error&) {
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Пример использования
    {
        using namespace std::literals;
        
        // На этой структуре будет проверяться работа умного указателя
        struct Object {
            Object() {
                std::cout << "Object is default constructed"s << std::endl;
            }
            void DoSomething() {
                std::cout << "Doing something"s << std::endl;
            }
            ~Object() {
                std::cout << "Object is destroyed"s << std::endl;
            }
        };

        // Сконструированный по умолчанию указатель ссылается на nullptr
        ScopedPtr<Object> empty_smart_ptr;
        // Перегруженный оператор приведения к типу bool вернёт false для пустого указателя
        assert(!empty_smart_ptr);

        ScopedPtr<Object> smart_ptr(new Object());
        // Перегруженный оператор bool вернёт true для указателя, ссылающегося на объект
        assert(smart_ptr);

        // Проверка оператора разыменования
        (*smart_ptr).DoSomething();
        // Проверка оператора доступа к членам класса
        smart_ptr->DoSomething();
    }
}