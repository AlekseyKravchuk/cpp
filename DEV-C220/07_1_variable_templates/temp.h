#pragma once

#include <cstddef>  // std::size_t

// объявление класса
template <typename T>
class A {
    T _t;
    static size_t _count;

   public:
    // совмещаем дефолтный конструктор и конструктор с параметром
    // дефолтным конструктором называется конструктор, у которого нет параметров или все параметры заданы по умолчанию
    A(const T& t = T{}) : _t(t) {
        ++_count;
    }

    // также учет экземляров класса нужно реализовать в конструкторе копирования
    // A(const T& other) {
    //     ++_count;
    // }

    ~A() {
        --_count;
    }

    static size_t GetCount() {
        return _count;
    }
};

// для статического счетчика в header'у нужно определить счетчик, иначе линкер будет выдавать ошибки
// это для общего случая - generic case
// для типа "double" компилятор будет использовать generic-шаблон
template <typename T>
size_t A<T>::_count;

// а это специализация для "int" шаблонного статического члена
template<> size_t A<int>::_count = 33;