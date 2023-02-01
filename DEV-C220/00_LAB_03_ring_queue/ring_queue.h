#include <algorithm>  // std::copy
#include <cassert>    // std::assert
#include <initializer_list>
#include <stdexcept>
#include <utility>  // std::move
#include <vector>

template <typename T>
class RingQueue {
   private:
    T* _ptr{nullptr};  // указатель на начало динамического массива
    int _size{};       // актуальное количество элементов в очереди
    int _capacity{};   // емкость (сколько выделено памяти)
    int _first{};      // индекс первого элемента в очереди (это тот элемент, который можно извлечь с помощью операции "pop")
    int _last{};       // индекс первого свободного элемента в очереди, в который можно записать значение: "enqueue (или "push")"
    static const int _delta{1};

   public:
    // базируется на том же параметре шаблона
    // класс - это только объявление
    class iterator {
        // данные и методы, реализующие функциональность итератора для кольцевой очереди
       private:
        const RingQueue* _storagePtr{nullptr};  // указатель на свой контейнер (итерируемая очередь)
        size_t _index{};                        // индекс текущего элемента

       public:
        // ============================== PUBLIC-методы ИТЕРАТОРА ==============================
        iterator() = default;

        iterator(const RingQueue* ringPtr, size_t index)
            : _storagePtr(ringPtr), _index(index) {}

        bool operator!=(const iterator& rhs) const noexcept {
            // return _index != rhs._index;

            // рассмотрим случай, если rhs - это ссылка на итератор другой очереди
            return (_storagePtr->_ptr + _index) != (rhs._storagePtr->_ptr + rhs._index);
        }

        T& operator*() {
            assert(_storagePtr);

            return *(_storagePtr->_ptr + _index);
        }

        const T& operator*() const {
            assert(_storagePtr);

            return *(_storagePtr->_ptr + _index);
        }

        // перемещение по УЖЕ СУЩЕСТВУЮЩИМ ЭЛЕМЕНТАМ
        iterator& operator++() noexcept {
            assert(_storagePtr);

            // if (++_index == _storagePtr->_capacity) {
            //     _index = 0;
            // }

            // условие можно заменить на результат взятия остатка от деления на "_capacity"
            _index = (_index + 1) % _storagePtr->_capacity;

            return *this;
        }
    };

    // ============================== PUBLIC-методы самой ОЧЕРЕДИ ==============================
   public:
    // так как класс сложный, реализуем «джентльменский» набор:
    RingQueue() = default;

    ~RingQueue() {
        delete[] _ptr;
    }

    // классический конструктор копирования
    RingQueue(const RingQueue& other) {
        _ptr = new T[other._size + 1];

        // std::copy(other.begin(), other.end(), newStoragePtr);  // TODO

        for (const auto& elm : other) {
            _ptr[_last] = elm;
            ++_last;
        }

        _size = other._size;
        _capacity = other._size + 1;
        _first = 0;
    }

    // оператор присваивания
    RingQueue& operator=(const RingQueue& other) {
        if (this != other) {
            if (_capacity < other._capacity) {
                // reallocation
            }
        }
    }

    RingQueue(RingQueue&&) {}

    RingQueue& operator=(RingQueue&&) {}

    RingQueue(const std::initializer_list<T> iniList) {
        try {
            _ptr = new T[iniList.size() + _delta];
            std::copy(iniList.begin(), iniList.end(), _ptr);
            _size = iniList.size();
            _capacity = iniList.size() + _delta;
            _first = 0;
            _last = _size - 1;
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

    // получить итератор на начало очереди
    iterator begin() const {
        return iterator(this, _first);
    }

    iterator end() const {
        return iterator(this, _last);
    }

    void push(const T& val) {
        if (_size + 1 == _capacity) {
            // перевыделяем память
            auto tmpPtr = new T[_capacity + _delta];
            for (int i = 0; i < _size; ++i) {
                *(tmpPtr + i) = std::move(*(_ptr + _first));  // конструкция "*(_ptr + first)" вернет ссылку, далее вызов перемещающего конструктора
                _first = (_first + 1) % _capacity;
            }

            std::swap(_ptr, tmpPtr);
            delete[] tmpPtr;

            // ========== атомарная операция добавления элемента ==========
            _first = 0;
            _last = _size;
            _capacity += _delta;

            // ============================================================
        }
        *(_ptr + _last) = val;  // эквивалентно: _ptr[_last]
        _last = (_last + 1) % _capacity;
        ++_size;
    }

    T pop() {
        if (_size > 0) {
            auto oldFirst = _first;
            _first = (_first + 1) % _capacity;
            --_size;

            return *(_ptr + oldFirst);
        } else {
            throw std::out_of_range("RingQueue is empty. pop() operation undefined."s);
        }
    }
};