#include <vector>

template <typename T>
class RingQueue {
   private:
    T* _ptr{nullptr};      // указатель на начало динамического массива
    int _size{};           // актуальное количество элементов в очереди
    int _capacity{};       // емкость (сколько выделено памяти)
    int idxFirst{};        // индекс первого элемента в очереди (это тот элемент, который можно извлечь с помощью операции "pop")
    int idxPastTheLast{};  // индекс первого свободного элемента в очереди, в который можно записать значение: "enqueue (или "push")"

   public:
    // базируется на том же параметре шаблона
    class iterator {
        // данные и методы, реализующие функциональность итератора для кольцевой очереди
       private:
        // индекс указываемого элемента
        size_t _index{};

        // указатель на свой контейнер
        const RingQueue* _storagePtr{nullptr};

       public:
        iterator() = default;

        iterator(const RingQueue* rinqPtr, size_t index)
            : _storagePtr(rinqPtr), _index(index) {}

        // Оператор проверки итераторов на неравенство (константный)
        bool operator!=(const iterator& rhs) const noexcept {
            return _index != rhs._index;
        }

        T& operator*() {
            assert(_storagePtr);

            return *(_storagePtr + _index);
        }

        const T& operator*() const {
            assert(_storagePtr);

            return *(_storagePtr + _index);
        }

        // перемещение по УЖЕ СУЩЕСТВУЮЩИМ ЭЛЕМЕНТАМ
        iterator& operator++() noexcept {
            assert(_storagePtr);

            if (++_index == _storagePtr->_capacity - 1) {
                _index = 0;
            }

            return *this;
        }

        // TODO: реализовать метод Push:
        // Ситуация 1: буфер уже полон, и инкремент итератора приведёт к переводу его
        // на позицию PastTheLast => нужно увеличивать capacity

        // Ситуация 2: в буфере спереди ещё есть место,но инкремент итератора приводит к
        // переводу его на позицию PastTheLast => нужно реализовать перескок
    };

   public:
    // так как класс сложный, реализуем «джентльменский» набор:
    RingQueue() {}
    ~RingQueue() {}

    RingQueue(const RingQueue&) {}
    RingQueue(RingQueue&&) {}
    
    RingQueue& operator=(const RingQueue&) {}
    RingQueue& operator=(RingQueue&&) {}

    // получить итератор на начало очереди
    iterator begin() const {
    }

    iterator end() const {
    }
};