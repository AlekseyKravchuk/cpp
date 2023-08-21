#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T>
class SimpleVector {
   public:
    SimpleVector() = default;
    explicit SimpleVector(size_t size);
    SimpleVector(const SimpleVector<T>& other);
    SimpleVector& operator=(const SimpleVector<T>& other);
    ~SimpleVector();

    T& operator[](size_t index);

    T* begin();
    T* end();

    const T* begin() const;
    const T* end() const;

    size_t Size() const;
    size_t Capacity() const;
    void PushBack(const T& value);

   private:
    T* _data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;
};

template <typename T>
SimpleVector<T>::SimpleVector(size_t size)
    : _data(new T[size]),
      _size(size),
      _capacity(size) {}

template <typename T>
SimpleVector<T>::SimpleVector(const SimpleVector<T>& other)
    : _data(new T[other._capacity]),
      _size(other._size),
      _capacity(other._capacity) {
    std::copy(other.begin(), other.end(), this->begin());
}

// copy assignment operator
template <typename T>
SimpleVector<T>& SimpleVector<T>::operator=(const SimpleVector<T>& other) {
    // 1) Проверяем, не выполняется ли присваивание массива самому себе, сравниваем адреса левого и правого аргументов операции присваивания.
    assert(_data != other._data);

    // 2) создаем временный вектор такого же размера, как и "other"
    // Выбрасывание исключения на этом этапе никак не повлияет на состояние текущего объекта (левый аргумент операции присваивания).
    SimpleVector<T> tmp_vec(other.Capacity());

    // 3) копируем в созданный временный вектор элементы из "other"
    std::copy(other.begin(), other.end(), tmp_vec.begin());

    // 4) перенаправляем значения указателей
    std::swap(_data, tmp_vec._data);

    // 5) копируем значения полей из other
    _size = other.Size();
    _capacity = other.Capacity();

    return *this;
}  // При выходе из функции "operator=" временный объект "tmp_vec" будет разрушен, память от предыдущего содержимого списка будет освобождена.

template <typename T>
SimpleVector<T>::~SimpleVector() {
    delete[] _data;
}

template <typename T>
T& SimpleVector<T>::operator[](size_t index) {
    return _data[index];
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
        T* ptr_new_data = new T[new_cap];
        std::copy(begin(), end(), ptr_new_data);
        std::swap(_data, ptr_new_data);
        delete[] ptr_new_data;
        _capacity = new_cap;
    }

    _data[_size++] = value;
}
