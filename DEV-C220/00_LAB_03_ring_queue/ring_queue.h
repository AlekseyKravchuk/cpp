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
#include <algorithm>
#include <cassert>
#include <cstddef>    // size_t
#include <exception>  // std::bad_alloc
#include <initializer_list>
#include <iostream>
#include <string>
#include <type_traits>

using namespace std::literals;

template <typename T>
class RingQueue {
   private:
    T* _rawPtr{nullptr};            // указатель на фактическое хранилище элементов (в heap'е)
    size_t _first{};                // индекс первого элемента
    size_t _last{};                 // индекс "past-the-end" элемента (он всегда пустой), в общем случае: _last = (_first + _size) % _capacity;
    size_t _size{};                 // фактическое количество элементов в очереди
    size_t _capacity{};             // количество зарезервированных под элементы мест в памяти
    static const size_t _delta{2};  // определяет на сколько больше памяти (в байтах) нужно заразервировать при перевыделении

    // вспомогательный метод, используется только в перегруженном "operator=="
    // вызывается только после проверки равенства размеров ("_size") соответствующих экземляров RingQueue<T>
    bool hasTheSameContent(const RingQueue<T> other) const {
        assert(other._rawPtr);

        // 1-ый вариант - "в лоб"
        // auto this_current = _first;
        // auto other_current = other._first;

        // for (size_t i = 0; i < _size; ++i) {
        //     if (_rawPtr[this_current] != other._rawPtr[other_current]) {
        //         return false;
        //     }
        //     this_current = (this_current + 1) % _capacity;
        //     other_current = (other_current + 1) % other._capacity;
        // }

        // 2-ой вариант с использованием итераторов
        for (auto it1 = begin(), it2 = other.begin(); it1 != end(); ++it1, ++it2) {
            *it1 = *it2;
        }

        return true;
    }

   public:
    class iterator {
       private:
        const RingQueue<T>* _prq{nullptr};  // указатель на константный экземляр "ring queue"
        size_t _index{};                    // индекс текущего элемента в экземляре "ring queue"

       public:
        // Для того, чтобы сделать вложенный класс "iterator" совместимым с STL-алгоритмами (header "algorithm"),
        // необходимо задать некоторые определения типов (посредством директивы "using")
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора - forward iterator (итератор, поддерживающий операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;

        // Тип элементов, по которым перемещается итератор
        using value_type = std::remove_cv_t<T>;

        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;

        // Тип указателя на итерируемое значение (указатель на константное значение)
        using pointer = const T*;

        // Тип ссылки на итерируемое значение
        using reference = const T&;

        iterator() = default;

        // этот конструктор будет вызываться только в методах "RingQueue<T>::begin()" и "RingQueue<T>::end()"
        // в качестве возвращаемого значения: в параметр "prq" будет передаваться "this", а в параметр "index" - "_first" или "_last".
        explicit iterator(const RingQueue<T>* prq, size_t index) : _prq(prq), _index(index) {}

        bool operator!=(const iterator& other) const {
            // 1-ый вариант - рассматривается случай, когда в качестве "other" был передан итератор на другую очередь:
            return (_prq->_rawPtr + _index) != (other._prq->_rawPtr + other._index);

            // 2-ой вариант (попроще):
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

        const T& operator*() const {
            return _prq->_rawPtr[_index];
        }

        T& operator*() {
            return _prq->_rawPtr[_index];
        }
    };  // ============ END of NESTED class "iterator" ============

    // ====================== public-методы RingQueue ======================
    RingQueue() = default;

    RingQueue(int num, T defaultValue = T{}) {
        _rawPtr = new T[num + 1];
        std::fill_n(_rawPtr, num, defaultValue);
        _first = 0;
        _last = _size = num;
        _capacity = _size + 1;
    }

    RingQueue(std::initializer_list<T> values) {
        _rawPtr = new T[values.size() + 1]{};
        _first = 0;
        _last = values.size();
        _size = values.size();
        _capacity = values.size() + 1;

        // 1-ый вариант копирования элементов
        // for (auto it = begin(); const auto& elm : values) {
        //     *it = elm;
        //     ++it;
        // }

        // 2-ой вариант копирования элементов
        std::copy(values.begin(), values.end(), begin());
    }

    // классический конструктор копирования с механизмом "Deep Copy"
    RingQueue(const RingQueue<T>& other) {
        _rawPtr = new T[other._size + 1];
        _first = 0;
        _size = other._size;
        _last = _size;
        _capacity = _size + 1;

        size_t start_index = 0;
        for (const auto& elm : other) {
            _rawPtr[start_index] = elm;
            ++start_index;
        }
    }

    // оператор присваивания
    RingQueue<T>& operator=(const RingQueue<T>& other) {
        if (this != &other) {
            if (_capacity < other._size + 1) {
                delete[] _rawPtr;
                _rawPtr = new T[other._size + 1];
                _capacity = other._size + 1;
            }

            _first = 0;
            _last = other._size;
            _size = other._size;

            // копируем элементы из "other._rawPtr" в "this->_rawPtr"
            // 1-ый вариант:
            // size_t index = 0;
            // for (const auto& elm : other) {
            //     _rawPtr[index++] = elm;
            // }

            // 2-ой вариант
            for (auto it = begin(); const auto& elm : other) {
                *it = elm;
                ++it;
            }
        }

        return *this;
    }

    // перемещающий оператор копирования
    RingQueue(RingQueue<T>&& other) {
        // перетаскиваем от "other" все ресурсы, а все ресурсы "other" зануляем
        _rawPtr = std::exchange(other._rawPtr, nullptr);
        _first = std::exchange(other._first, 0);
        _last = std::exchange(other._last, 0);
        _size = std::exchange(other._size, 0);
        _capacity = std::exchange(other._capacity, 0);
    }

    // перемещающий оператор присваивания
    RingQueue<T>& operator=(RingQueue<T>&& other) {
        if (this != &other) {
            delete[] _rawPtr;

            _rawPtr = std::exchange(other._rawPtr, nullptr);
            _first = std::exchange(other._first, 0);
            _last = std::exchange(other._last, 0);
            _size = std::exchange(other._size, 0);
            _capacity = std::exchange(other._capacity, 0);
        }

        return *this;
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
        if (_size == _capacity - 1) {  // буфер заполнен, осталось только1 место под фиктивный элемент,
                                       // который выступает "past the end" => требуется перевыделение памяти
            auto new_capacity = _capacity + _delta;
            T* newRawPtr = new T[new_capacity];  // выделяем памяти под ("old_capacity" + "_delta") элементов типа "T"

            // копируем "_size" элементов, делаем это эффективно, т.е. через перемещение (в предположении, что для типа "T" реализован move copy constructor )
            for (size_t i = 0; i < _size; ++i) {
                newRawPtr[i] = std::move(_rawPtr[_first]);
                _first = (_first + 1) % _capacity;
            }

            // теперь можно освободить память, выделенную под старое хранилище
            delete[] _rawPtr;

            // используем setter "std::exchange"
            // теперь "_rawPtr" указывает на новое хранилище в "heap'e", а "newRawPtr" зануляем для предотвращения его повторного использования
            _rawPtr = std::exchange(newRawPtr, nullptr);

            // осталось внести изменения в соответствующие поля экземпляра класса RingQueue<T>
            _first = 0;                // явно прописываем для наглядности того, что запись в новое хранилище велась с начала
            _last = _size;             // индекс последнего элемента равен "_size", т.к. "закольцовки" ещё нет и инкремент количества уже выполнен
            _capacity = new_capacity;  // обновляем "_capacity"
        }

        _rawPtr[_last] = val;
        ++_size;
        _last = (_last + 1) % _capacity;
    }

    T pop() {
        assert(_rawPtr);  // проверяем, что "_rawPtr" ненулевой

        auto valToReturn = _rawPtr[_first];
        _first = (_first + 1) % _capacity;
        --_size;

        return valToReturn;
    }

    // проверка на равенство 2-х очередей, в стиле "DEEP operation"
    // т.е. для выполнения условия должно быть выполнено равенство всех членов класса,
    // а также значений, хранящихся в heap'e
    bool operator==(const RingQueue<T>& other) const {
        return _size == other._size && hasTheSameContent(other);
    }

    // используется только для проверки корректности работы перемещающего конструктора копирования
    bool isEmpty() const {
        return _size == 0;
    }

    void PrintRingQueueState() {
        std::cout << "_first = " << GetFirst() << ", "s
                  << "_last = " << GetLast() << ", "s
                  << "_size = " << GetSize() << ", "s
                  << "_capacity = " << GetCapacity() << ", "s
                  << std::endl;
        std::cout << std::string(20, '=') << std::endl;
    }
};
