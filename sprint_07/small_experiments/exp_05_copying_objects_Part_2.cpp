#include <cassert>
#include <iostream>
#include <new>
#include <stdexcept>  //std::logic_error
#include <string>

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
   public:
    // Конструктор по умолчанию создаёт нулевой указатель,
    // так как поле _ptr имеет значение по умолчанию nullptr
    ScopedPtr() = default;

    // Создаёт указатель, ссылающийся на переданный rawPtr.
    // rawPtr ссылается либо на объект, созданный в куче при помощи new, либо является нулевым указателем
    // Спецификатор noexcept обозначает, что метод не бросает исключений
    explicit ScopedPtr(T* rawPtr) noexcept {
        _ptr = rawPtr;
    }

    // Удаляем у класса конструктор копирования
    ScopedPtr(const ScopedPtr&) = delete;

    // Деструктор. Удаляет объект, на который ссылается умный указатель.
    ~ScopedPtr() {
        if (_ptr) {
            delete _ptr;
        }
    }

    // Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель на какой-либо объект
    // Оператор преобразования к типу bool объявлен explicit, чтобы запретить неявное преобразование указателя в bool,
    // которое практически всегда нежелательно.
    // На проверке указателя в условии if это не отразится.
    explicit operator bool() const noexcept {
        return _ptr != nullptr;
    }

    // Оператор разыменования возвращает ссылку на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T& operator*() const {
        using namespace std::literals;

        if (_ptr) {
            return *_ptr;
        } else {
            throw std::logic_error("Invalid dereferencing of pointer using arrow operator"s);
        }
    }

    // Возвращает указатель, хранящийся внутри ScopedPtr
    T* GetRawPtr() const noexcept {
        return _ptr;
    }

    // Оператор -> должен возвращать указатель на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T* operator->() const {
        using namespace std::literals;

        if (_ptr) {
            return _ptr;
        } else {
            throw std::logic_error("Invalid dereferencing of pointer"s);
        }
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

void Test1() {
    // Вспомогательный "шпион", позволяющий узнать о своём удалении
    struct DeletionSpy {
        explicit DeletionSpy(bool& isDeleted) : _isDeleted(isDeleted) {}
        ~DeletionSpy() { _isDeleted = true; }

        bool& _isDeleted;
    };

    // Проверяем автоматическое удаление
    {
        bool isDeleted = false;
        {
            // настраиваем "шпион", чтобы при своём удалении он выставил isDeleted в true
            DeletionSpy* rawPtr = new DeletionSpy(isDeleted);
            ScopedPtr<DeletionSpy> p(rawPtr);
            assert(p.GetRawPtr() == rawPtr);
            assert(!isDeleted);
            // При выходе из блока деструктор p должен удалить "шпиона"
        }
        // Если деструктор умного указателя работает правильно, шпион перед своей "смертью"
        // должен выставить is_deleted в true
        assert(isDeleted);
    }

    // Проверяем работу метода Release
    {
        bool is_deleted = false;
        DeletionSpy* raw_ptr = new DeletionSpy(is_deleted);
        {
            ScopedPtr<DeletionSpy> scoped_ptr(raw_ptr);
            assert(scoped_ptr.Release() == raw_ptr);
            assert(scoped_ptr.GetRawPtr() == nullptr);
            // После Release умный указатель не ссылается на объект и не удаляет его при своём удалении
        }
        assert(!is_deleted);
        delete raw_ptr;
        assert(is_deleted);
    }
}

void TestBool() {
    // Проверка работы оператора приведения к типу bool
    {
        // Для нулевого указателя приведение к типу bool возвращает false
        const ScopedPtr<int> emptyPtr;
        assert(!emptyPtr);

        // Для ненулевого указателя приведение к типу bool возвращает true
        const ScopedPtr<int> ptr2ExistingObject(new int(0));
        assert(ptr2ExistingObject);

        static_assert(noexcept(static_cast<bool>(ptr2ExistingObject)));
    }
}

void TestDereferencingOperator() {
    // Проверка работы оператора разыменования *
    {
        std::string* rawPtr = new std::string("hello");
        ScopedPtr<std::string> smartPtr(rawPtr);
        // Ссылка, возвращаемая оператором разыменования, должна ссылаться на объект,
        // на который указывает умный указатель
        assert(&*smartPtr == rawPtr);

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
}

void TestArrowOperator() {
    // Проверка работы оператора ->
    {
        using namespace std::literals;

        std::string* rawPtr = new std::string("hello"s);
        ScopedPtr<std::string> smartPtr(rawPtr);

        // Доступ к членам класса через умный указатель должен быть аналогичен
        // доступу через "сырой" указатель
        assert(smartPtr->data() == rawPtr->data());

        try {
            ScopedPtr<std::string> empty_ptr;
            // При попытке разыменовать пустой указатель должно быть выброшено
            // исключение logic_error
            empty_ptr->clear();
            // Сюда попасть мы не должны
            assert(false);
        } catch (const std::logic_error&) {
            // мы там, где нужно
            std::cout << "std::logic_error was thrown."s << std::endl;
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

// Этот main тестирует класс ScopedPtr
int main() {
    // Test1();
    // TestBool();
    // TestDereferencingOperator();
    TestArrowOperator();
}