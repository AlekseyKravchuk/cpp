// Задание. Разработайте класс, который реализует функциональность очереди с элементами любого типа.
// Данные требуется хранить в динамическом массиве (в heap'e), при этом использовать массив КАК ЦИКЛИЧЕСКИЙ БУФЕР.
// Пояснение: поскольку очередь – это специфическая структура данных, для которой новые данные помещаются в конец в порядке поступления (FIFO),
// а  «старые» данные изымаются из начала очереди.
// Рассмотрим возможные ситуации при работе с кольцевым буфером (очередью):
// Ситуация №1: последний элемент массива задействован, а в начале очереди уже имеются свободные места.
//              Таким образом, при поступлении очередного элемента (push, enqueue) требуется «закольцовывать» буфер,
//              продолжая запись элементов с нулевого элемента.
// Ситуация №2: вызывается push (enqueue), а свободных элементов не осталось => требуется перевыделить память с запасом.

#include <algorithm>  // std::copy
#include <cassert>
#include <cstddef>    // size_t
#include <exception>  // std::bad_alloc
#include <initializer_list>
#include <string>

using namespace std::literals;

template <typename T>
class RingQueue {
   private:
    T* _rawPtr{nullptr};
    size_t _first{};
    size_t _last{};                 // индекс на последний (резервный, всегда пустой) элемент: _last = _first + _size;
    size_t _size{};                 // фактическое количество элементов в очереди
    size_t _capacity{};             // количество зарезервированных под элементы мест в памяти
    static const size_t _delta{1};  // определяет на сколько больше памяти нужно заразервировать при перевыделении

   public:
    // итератор - это, по сути, указатель на ТЕКУЩИЙ элемент в контейнере
    class iterator {
       private:
        const RingQueue<T>* _prq{nullptr};  // указатель на константный экземляр "ring queue"
        size_t _index{};                    // индекс текущего элемента в экземляре "ring queue"
       public:
        iterator() = default;

        // этот конструктор будет вызываться только в методах "RingQueue<T>::begin()" и "RingQueue<T>::end()"
        // в качестве возвращаемого значения: в prq будет передаваться "this", а в "index" - "_first" и "_last" соответственно.
        explicit iterator(const RingQueue<T>* prq, size_t index) : _prq(prq), _index(index) {}

        bool operator!=(const iterator& other) const {
            // рассматривается случай, когда в качестве "other" был передан итератор на другую очередь
            return (_prq->_rawPtr + _index) != (other._prq->_rawPtr + other._index);
            // return _index != other._index;
        }

        // перегрузка оператора "prefix increment"
        // инкремент итератора (++it) приводит к инкременту его "_index", но при этом нужно учитывать, что у нас циклическая очередь
        iterator& operator++() {
            assert(_prq);

            // значения индекса будут находиться в диапазоне [0, 1, 2, ..., (_capacity - 1)]
            _index = (_index + 1) % _prq->_capacity;
            return *this;
        }

        T operator*() const {
            return _prq->_rawPtr[_index];
        }
    };

    // ====================== public-методы RingQueue ======================
    RingQueue() = default;

    RingQueue(std::initializer_list<T> values) {
        _rawPtr = new T[values.size() + 1]{};

        // 1-ый вариант копирования элементов
        for (const auto& elm : values) {
            _rawPtr[_last++] = elm;
        }

        // 2-ой вариант копирования элементов
        // std::copy(values.begin(), values.end(), _rawPtr);
        // _last = values.size();

        _size = values.size();
        _capacity = values.size() + 1;
    }

    ~RingQueue() {
        delete[] _rawPtr;
    };

    iterator begin() const {
        return iterator{this, _first};
    }

    iterator end() const {
        return iterator{this, _last};
    }

    void push(const T& val) {
        // TO DO:
        if (_size == _capacity - 1) {
            // если буфер полон, требуется перевыделение памяти
            T* _newStorage = new T[_capacity + _delta];

            // TO DO: копируем содержимое
        }
    }
};