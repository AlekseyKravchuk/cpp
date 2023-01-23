#include <vector>

template <typename T>
class RingQueue {
   private:
    T* _ptr{nullptr};
    int _cap{};
    int _sz{};

    int indexFirst{};
    int indexPastTheLast{};

   public:
    // базируется на том же параметре шаблона
    class QueueIterator {
        friend RingQueue;

       private:
        // индекс указываемого элемента
        size_t _index{};

        // указатель на свой контейнер
        const RingQueue* _storagePtr{nullptr};

       public:
        QueueIterator() = default;

        QueueIterator(const RingQueue* rinqPtr, size_t index)
            : _storagePtr(rinqPtr), _index(index) {}

        // Оператор проверки итераторов на неравенство (константный)
        bool operator!=(const QueueIterator& rhs) const noexcept {
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
        QueueIterator& operator++() noexcept {
            assert(_storagePtr);

            if (++_index == _storagePtr->_cap - 1) {
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
};