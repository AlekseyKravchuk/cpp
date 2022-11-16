#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

template <typename T>
class PtrVector {
   public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        _items.reserve(other.GetItems().size());

        for (T* ptr: other.GetItems()) {
            T* newPtr = ptr ? new T(*ptr) : nullptr;
            _items.push_back(newPtr);
        }
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели, в векторе _items
    ~PtrVector() {
        for (T* ptr : GetItems()) {
            if (ptr) {
                delete ptr;
            }
        }
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
};

// Эта функция main тестирует шаблон класса PtrVector
int main() {
    // Вспомогательный "шпион", позволяющий узнать о своём удалении
    struct DeletionSpy {
        explicit DeletionSpy(bool& isDeleted) : _isDeleted(isDeleted) {}
        ~DeletionSpy() { _isDeleted = true; }
        
        bool& _isDeleted;
    };

    // Проверяем удаление элементов
    {
        bool spy1IsDeleted = false;
        DeletionSpy* ptr1 = new DeletionSpy(spy1IsDeleted);
        {
            PtrVector<DeletionSpy> ptr_vector;
            ptr_vector.GetItems().push_back(ptr1);
            assert(!spy1IsDeleted);

            // Константная ссылка на ptr_vector
            const auto& const_ptr_vector_ref(ptr_vector);
            // И константная, и неконстантная версия GetItems
            // должны вернуть ссылку на один и тот же вектор
            assert(&const_ptr_vector_ref.GetItems() == &ptr_vector.GetItems());
        }
        // При разрушении ptr_vector должен удалить все объекты, на которые
        // ссылаются находящиеся внутри него указателели
        assert(spy1IsDeleted);
    }

    // Вспомогательный «шпион», позволяющий узнать о своём копировании
    struct CopyingSpy {
        explicit CopyingSpy(int& copyCount) : _copyCount(copyCount) { }

        CopyingSpy(const CopyingSpy& rhs) : _copyCount(rhs._copyCount) {
            ++_copyCount;
        }

        int& _copyCount;
    };

    // Проверяем копирование элементов при копировании массива указателей
    {
        // 10 элементов
        vector<int> copyCounters(10);

        PtrVector<CopyingSpy> ptrVector;
        // Подготавливаем оригинальный массив указателей
        for (auto& counter : copyCounters) {
            ptrVector.GetItems().push_back(new CopyingSpy(counter));
        }

        // Последний элемент содержит нулевой указатель
        ptrVector.GetItems().push_back(nullptr);

        auto ptrVectorCopy(ptrVector);
        // Количество элементов в копии равно количеству элементов оригинального вектора
        assert(ptrVectorCopy.GetItems().size() == ptrVector.GetItems().size());

        // копия должна хранить указатели на новые объекты
        assert(ptrVectorCopy.GetItems() != ptrVector.GetItems());
        // Последний элемент исходного массива и его копии - нулевой указатель
        assert(ptrVectorCopy.GetItems().back() == nullptr);
        // Проверяем, что элементы были скопированы (копирующие шпионы увеличивают счётчики копий).
        assert(all_of(copyCounters.begin(), copyCounters.end(), [](int counter) {
            return counter == 1;
        }));
    }
}