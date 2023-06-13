#pragma once

#include <cstddef>  // size_t

template <typename T>
class SimpleVector {
   public:
    explicit SimpleVector(size_t size) : _size(size),
                                         _data(new T[size]) {}

    ~SimpleVector() { delete[] _data; }

    // оператор доступа к элементу вектора по индексу (оператор индексирования, subscript operator)
    T& operator[](size_t index) { return _data[index]; }

    // в качестве возвращаемого значения можно использовать УКАЗАТЕЛЬ на первый элемент нашего вектора
    T* begin() { return _data; }
    const T* begin() const { return _data; }

    // в качестве возвращаемого значения можно использовать УКАЗАТЕЛЬ на элемент ЗА ПОСЛЕДНИМ (ont-past-the-end)
    T* end() { return _data + _size; }
    const T* end() const { return _data + _size; }

   private:
    size_t _size;
    T* _data;
};
