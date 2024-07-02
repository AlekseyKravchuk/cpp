#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

template <typename T>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const T& val, Node* next) : value(val), nextNode(next) {}
        T value;
        Node* nextNode = nullptr;
    };

   public:
    SingleLinkedList() = default;

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return _size;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return _size == 0;
    }

    void PushFront(const T& val) {
        Node* insertedNodePtr = new Node(val, _head.nextNode);
        _head.nextNode = insertedNodePtr;

        ++_size;
    }

    void Clear() noexcept {
        while (_head.nextNode) {
            Node* firstNodePtr = _head.nextNode;
            auto secondNodePtr = firstNodePtr->nextNode ? firstNodePtr->nextNode : nullptr;
            _head.nextNode = secondNodePtr;
            delete firstNodePtr;
        }

        _size = 0;
    }

   private:
    Node _head = Node();  // Фиктивный узел, используется для вставки "перед первым элементом"
    size_t _size = 0;
};

// Эта функция тестирует работу SingleLinkedList
void Test1() {
    // Шпион, следящий за своим удалением
    struct DeletionSpy {
        DeletionSpy() = default;
        explicit DeletionSpy(int& instanceCounter) noexcept : _instanceCounterPtr(&instanceCounter) {
            OnAddInstance();
        }

        DeletionSpy(const DeletionSpy& other) noexcept : _instanceCounterPtr(other._instanceCounterPtr) {
            OnAddInstance();
        }

        DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
            if (this != &rhs) {
                auto rhs_copy(rhs);
                std::swap(_instanceCounterPtr, rhs_copy._instanceCounterPtr);
            }

            return *this;
        }

        ~DeletionSpy() {
            OnDeleteInstance();
        }

       private:
        void OnAddInstance() noexcept {
            if (_instanceCounterPtr) {
                ++(*_instanceCounterPtr);
            }
        }

        void OnDeleteInstance() noexcept {
            if (_instanceCounterPtr) {
                assert(*_instanceCounterPtr != 0);
                --(*_instanceCounterPtr);
            }
        }

       private:
        int* _instanceCounterPtr = nullptr;
    };

    // Проверка вставки в начало
    {
        SingleLinkedList<int> l;
        assert(l.IsEmpty());
        assert(l.GetSize() == 0u);

        l.PushFront(0);
        l.PushFront(1);
        assert(l.GetSize() == 2);
        assert(!l.IsEmpty());

        l.Clear();
        assert(l.GetSize() == 0);
        assert(l.IsEmpty());
    }

    // Проверка фактического удаления элементов
    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            SingleLinkedList<DeletionSpy> list;
            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});

            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
            list.Clear();
            assert(item0_counter == 0);
            assert(item1_counter == 0);
            assert(item2_counter == 0);

            list.PushFront(DeletionSpy{item0_counter});
            list.PushFront(DeletionSpy{item1_counter});
            list.PushFront(DeletionSpy{item2_counter});
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
        }
        assert(item0_counter == 0);  // <== ЗАВАЛИВАЕТСЯ ЗДЕСЬ!!!
        assert(item1_counter == 0);
        assert(item2_counter == 0);
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    {
        bool exception_was_thrown = false;
        // Последовательно уменьшаем счётчик копирований до нуля, пока не будет выброшено исключение
        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            // Создаём непустой список
            SingleLinkedList<ThrowOnCopy> list;
            list.PushFront(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.PushFront(ThrowOnCopy(copy_counter));
                // Если метод не выбросил исключение, список должен перейти в новое состояние
                assert(list.GetSize() == 2);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                // После выбрасывания исключения состояние списка должно остаться прежним
                assert(list.GetSize() == 1);
                break;
            }
        }
        assert(exception_was_thrown);
    }
}

int main() {
    Test1();
}