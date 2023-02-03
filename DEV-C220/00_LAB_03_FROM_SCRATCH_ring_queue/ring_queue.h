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
    T* _rawPtr{nullptr};            // указатель на фактическое хранилище элементов (в heap'е)
    size_t _first{};                // индекс первого элемента
    size_t _last{};                 // индекс "past-the-end" элемента (он всегда пустой), в общем случае: _last = (_first + _size) % _capacity;
    size_t _size{};                 // фактическое количество элементов в очереди
    size_t _capacity{};             // количество зарезервированных под элементы мест в памяти
    static const size_t _delta{2};  // определяет на сколько больше памяти нужно заразервировать при перевыделении

   public:
    class iterator {
       private:
        const RingQueue<T>* _prq{nullptr};  // указатель на константный экземляр "ring queue"
        size_t _index{};                    // индекс текущего элемента в экземляре "ring queue"
       public:
        iterator() = default;

        // этот конструктор будет вызываться только в методах "RingQueue<T>::begin()" и "RingQueue<T>::end()"
        // в качестве возвращаемого значения: в параметр "prq" будет передаваться "this", а в параметр "index" - "_first" или "_last".
        explicit iterator(const RingQueue<T>* prq, size_t index) : _prq(prq), _index(index) {}

        bool operator!=(const iterator& other) const {
            // рассматривается случай, когда в качестве "other" был передан итератор на другую очередь:
            return (_prq->_rawPtr + _index) != (other._prq->_rawPtr + other._index);

            // вариант попроще:
            // return _index != other._index;
        }

        // перегрузка оператора "prefix increment"
        // инкремент итератора (++it) приводит к инкременту его "_index", но при этом нужно учитывать, что у нас циклическая очередь
        // => используем операцию по модулю "_capacity"
        iterator& operator++() {
            assert(_prq);

            // допустимые значения индекса будут находиться в диапазоне [0, 1, 2, ..., (_capacity - 1)]
            _index = (_index + 1) % _prq->_capacity;
            return *this;
        }

        T operator*() const {
            return _prq->_rawPtr[_index];
        }
    };  // ============ END of class iterator ============

    // ====================== public-методы RingQueue ======================
    RingQueue() = default;

    RingQueue(std::initializer_list<T> values) {
        _rawPtr = new T[values.size() + 1]{};

        // 1-ый вариант копирования элементов
        for (const auto& elm : values) {
            _rawPtr[_last++] = elm;
        }

        // 2-ой вариант копирования элементов - TODO!
        // std::copy(values.begin(), values.end(), _rawPtr);
        // _last = values.size();

        _size = values.size();
        _capacity = values.size() + 1;
    }

    // классический конструктор копирования с механизмом "Deep Copy"
    // TODO:
    RingQueue(const RingQueue<T>& other) {
        _rawPtr = new T[other._capacity];

        // копируем "_size" элементов
        for (size_t i = 0; i < _size; ++i) {
            _rawPtr[i] = other._rawPtr[_first];
            _first = (_first + 1) % _capacity;
        }
        // нужно сделать ТОЧНО ТАКУЮ ЖЕ ОЧЕРЕДЬ, т.е. с сохранением её текущих состояний
        // осталось скопировать соответствующие поля из экземпляра "other" в "*this"
        _first = 0;                   // явно прописываем для наглядности того, что запись в новое хранилище велась с начала
        _last = other._size;          // индекс последнего элемента равен "_size", т.к. "закольцовки" ещё нет и инкремент количества уже выполнен
        _size = other._size;
        _capacity = other._capacity;  // обновляем "_capacity"
    }

    ~RingQueue() {
        delete[] _rawPtr;
    };

    auto GetFirst() const {
        return _first;
    }

    auto GetLast() const {
        return _last;
    }

    auto GetSize() const {
        return _size;
    }

    auto GetCapacity() const {
        return _capacity;
    }

    iterator begin() const {
        return iterator{this, _first};
    }

    iterator end() const {
        return iterator{this, _last};
    }

    void push(const T& val) {
        if (_size == _capacity - 1) {  // буфер заполнен, осталось 1 место под фиктивный элемент, который выступает "past the end"
            // требуется перевыделение памяти
            auto new_capacity = _capacity + _delta;
            T* newRawPtr = new T[new_capacity];  // выделяем памяти под ("old_capacity" + "_delta") элементов типа "T"

            // копируем "_size" элементов, делаем это эффективно, т.е. через перемещение (в предположении, что для типа "T" реализован move copy constructor )
            for (size_t i = 0; i < _size; ++i) {
                newRawPtr[i] = std::move(_rawPtr[_first]);
                _first = (_first + 1) % _capacity;
            }
            // наконец-то добавляем элемент
            newRawPtr[_first] = val;
            ++_size;  // добавили элемент => сразу делаем инкремент кол-ва элементов, чтобы не забыть

            // теперь можно освободить память, выделенную под старое хранилище
            delete[] _rawPtr;

            // используем setter "std::exchange"
            // теперь поле "_rawPtr" теперь указывает на новое хранилище в "heap'e", а "newRawPtr" зануляем на всякий случай
            _rawPtr = std::exchange(newRawPtr, nullptr);

            // осталось внести изменения в соответствующие поля экземпляра класса RingQueue<T>
            _first = 0;                // явно прописываем для наглядности того, что запись в новое хранилище велась с начала
            _last = _size;             // индекс последнего элемента равен "_size", т.к. "закольцовки" ещё нет и инкремент количества уже выполнен
            _capacity = new_capacity;  // обновляем "_capacity"

        } else {
            // в буфере ещё есть свободные места, можем добавлять элемент в текущее хранилище,
            // в результате произойдет инкремент индекса "_last"
            _rawPtr[_last] = val;
            ++_size;
            _last = (_last + 1) % _capacity;
        }
    }

    T& pop() {
        assert(_rawPtr);  // проверяем, что "_rawPtr" ненулевой
        auto saved_first = _first;
        _first = (_first + 1) % _capacity;
        --_size;

        return _rawPtr[saved_first];
    }
};