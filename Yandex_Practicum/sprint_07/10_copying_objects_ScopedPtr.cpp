#include <cassert>
#include <new>

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

// Этот main тестирует класс ScopedPtr
int main() {
    // Вспомогательный "шпион", позволяющий узнать о своём удалении
    struct DeletionSpy {
        explicit DeletionSpy(bool& isDeleted) : _isDeleted(isDeleted) { }
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