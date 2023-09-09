#include <algorithm>
#include <utility>  // std::move
// #include <cstdint>

// Реализуйте SimpleVector в этом файле и отправьте его на проверку

template <typename T>
class SimpleVector {
   public:
    SimpleVector() = default;
    explicit SimpleVector(size_t size);
    SimpleVector(const SimpleVector<T>& other);
    SimpleVector<T>& operator=(const SimpleVector<T>& other);

    SimpleVector(SimpleVector<T>&& other);
    SimpleVector<T>& operator=(SimpleVector<T>&& other);
    
    ~SimpleVector();

    T& operator[](size_t index);

    // ============================== Пара неконстантных методов "begin" и "end" ===============================
    T* begin();
    T* end();
    // =========================================================================================================

    // ======== Пара КОНСТАНТНЫХ методов "begin" и "end" (возвращают указатель на КОНСТАНТНОЕ значение) ========
    const T* begin() const;
    const T* end() const;
    // =========================================================================================================

    size_t Size() const;
    size_t Capacity() const;
    void PushBack(const T& value);
    void PushBack(T&& value);
    void Swap(SimpleVector<T>& rhs);

    // При необходимости перегрузите существующие публичные методы

   private:
    T* _data{nullptr};
    size_t _size{};
    size_t _capacity{};
};

template <typename T>
SimpleVector<T>::SimpleVector(size_t size)
    : _data(new T[size]),
      _size(size),
      _capacity(size) {}

template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector& other)
    : _data(new T[other.Capacity()]),
      _size(other.Size()),
      _capacity(other.Capacity()) {
    std::move(other.begin(), other.end(), begin());
}

template <typename T>
SimpleVector<T>::SimpleVector(SimpleVector<T>&& other)
    : _data(other._data),
      _size(other._size),
      _capacity(other._capacity) {
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}

template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(const SimpleVector<T>& rhs) {
    if (_data == rhs._data) {
        return *this;
    }

    SimpleVector<T> tmp(std::move(rhs));  // идиома COPY-AND-SWAP: 1) создаём временный вектор с помощью конструктора копирования
    Swap(tmp);                            //                       2) обмениваем поля своего объекта с полями временного объекта

    // Применение идиомы COPY-AND-SWAP позволяет достичь следующих двух целей:
    //  - избегаем дублирования кода в конструкторе копирования и операторе присваивания
    //  - обеспечиваем согласованное поведение конструктора копирования и оператора присваивания

    return *this;
}

template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(SimpleVector<T>&& other) {
    delete[] _data;

    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;

    return *this;
}

template <typename T>
SimpleVector<T>::~SimpleVector() {
    delete[] _data;
}

// оператор доступа к элементу вектора по индексу (оператор индексирования, subscript operator / array index operator)
template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
    return _data[index];  // эквивалентно "return *(_data + index);"
}

// ======== Пара неконстантных методов "begin" и "end" ========
template <typename T>
T* SimpleVector<T>::begin() { return _data; }

template <typename T>
T* SimpleVector<T>::end() { return _data + _size; }  // в качестве возвр. значения используем УКАЗАТЕЛЬ на one-past-the-end
// ============================================================

// ======== Пара КОНСТАНТНЫХ методов "begin" и "end" (возвращают указатель на КОНСТАНТНОЕ значение) ========
template <typename T>
const T* SimpleVector<T>::begin() const { return _data; }

template <typename T>
const T* SimpleVector<T>::end() const { return _data + _size; }
// =========================================================================================================

template <typename T>
size_t SimpleVector<T>::Size() const { return _size; }

template <typename T>
size_t SimpleVector<T>::Capacity() const { return _capacity; }

template <typename T>
void SimpleVector<T>::PushBack(const T& value) {
    if (_size >= _capacity) {
        size_t new_cap = (_capacity == 0) ? 1 : (2 * _capacity);
        T* ptr_newly_allocated = new T[new_cap];
        std::copy(begin(), end(), ptr_newly_allocated);
        std::swap(_data, ptr_newly_allocated);
        delete[] ptr_newly_allocated;
        _capacity = new_cap;
    }

    _data[_size++] = value;
}

template <typename T>
void SimpleVector<T>::PushBack(T&& value) {
    if (_size >= _capacity) {
        size_t new_capacity = (_capacity == 0) ? 1 : (2 * _capacity);
        T* ptr_newly_allocated = new T[new_capacity];
        std::move(begin(), end(), ptr_newly_allocated);
        std::swap(_data, ptr_newly_allocated);
        delete[] ptr_newly_allocated;
        _capacity = new_capacity;
    }

    _data[_size++] = std::move(value);
}

template <typename T>
void SimpleVector<T>::Swap(SimpleVector<T>& rhs) {
    std::swap(_data, rhs._data);
    std::swap(_size, rhs._size);
    std::swap(_capacity, rhs._capacity);
}
