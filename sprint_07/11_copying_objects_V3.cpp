#include <cassert>
#include <iostream>
#include <new>
#include <stdexcept>  // std::logic_error
#include <string>

using namespace std;

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
   public:
    // Конструктор по умолчанию создаёт нулевой указатель,
    // так как поле _ptr имеет значение по умолчанию nullptr
    ScopedPtr() = default;

    // Создаёт указатель, ссылающийся на переданный rawPtr.
    // rawPtr ссылается либо на объект, созданный в куче при помощи new,
    // либо является нулевым указателем
    // Спецификатор noexcept обозначает, что метод не бросает исключений
    explicit ScopedPtr(T* rawPtr) noexcept {
        _ptr = rawPtr;
    }

    // Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
    // на какой-либо объект
    explicit operator bool() const noexcept {
        return (_ptr != nullptr) ? true : false;
    }

    // Оператор разыменования возвращает ссылку на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T& operator*() const {
        if (_ptr == nullptr) {
            throw std::logic_error("Null pointer"s);
        }

        return *_ptr;
    }

    // Оператор -> должен возвращать указатель на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T* operator->() const {
        if (_ptr == nullptr) {
            throw std::logic_error("Null pointer"s);
        }

        return _ptr;
    }

    // Удаляем у класса конструктор копирования
    ScopedPtr(const ScopedPtr&) = delete;

    // Деструктор. Удаляет объект, на который ссылается умный указатель.
    ~ScopedPtr() {
        if (_ptr) {
            delete _ptr;
        }
    }

    // Возвращает указатель, хранящийся внутри ScopedPtr
    T* GetRawPtr() const noexcept {
        return _ptr;
    }

    // Прекращает владение объектом, на который ссылается умный указатель.
    // Возвращает прежнее значение "сырого" указателя и устанавливает поле _ptr в null
    T* Release() noexcept {
        T* tmp = _ptr;
        _ptr = nullptr;

        return tmp;
    }

   private:
    T* _ptr = nullptr;
};

/* void Fn(int i);

int main() {
    ScopedPtr<Object> p(new Object());
    Fn(p);  // Когда operator bool объявлен explicit, здесь будет ошибка компиляции
            // Без explicit в Fn будет передано значение 1
} */

// Этот main тестирует класс ScopedPtr
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
        string* raw_ptr = new string("hello");
        ScopedPtr<string> smart_ptr(raw_ptr);
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
        string* raw_ptr = new string("hello");
        ScopedPtr<string> smart_ptr(raw_ptr);
        // Доступ к членам класса через умный указатель должен быть аналогичен
        // доступу через "сырой" указатель
        assert(smart_ptr->data() == raw_ptr->data());

        try {
            ScopedPtr<string> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            empty_ptr->clear();
            // Сюда попасть мы не должны
            assert(false);
        } catch (const logic_error&) {
            // мы там, где нужно
        } catch (...) {
            // Других исключений выбрасываться не должно
            assert(false);
        }
    }

    // Пример использования
    {
        // На этой структуре будет проверяться работа умного указателя
        struct Object {
            Object() {
                cout << "Object is default constructed"s << endl;
            }
            void DoSomething() {
                cout << "Doing something"s << endl;
            }
            ~Object() {
                cout << "Object is destroyed"s << endl;
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