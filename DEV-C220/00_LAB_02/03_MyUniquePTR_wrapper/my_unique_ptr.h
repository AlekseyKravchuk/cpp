#pragma once

#include <iostream>
#include <stdexcept>  // std::logic_error
#include <utility>    // std::exchange

template <typename T>
class MyUniquePTR {
   private:
    T* _rawPtr{nullptr};

   public:
    // конструктор по умолчанию создает нулевой указатель,
    // т.к. поле "_ptr" имеет значение по умолчанию "nullptr"
    MyUniquePTR() = default;

    // Создаёт указатель, хранящий адрес, переданный в "rawPtr".
    // "rawPtr" указывает либо на объект, созданный в куче при помощи new, либо является нулевым указателем
    // Спецификатор noexcept обозначает, что метод не бросает исключений
    explicit MyUniquePTR(T* rawPtr) noexcept {
        _rawPtr = rawPtr;
    }

    // Удаляем у класса "MyUniquePTR" конструктор копирования
    MyUniquePTR(const MyUniquePTR&) = delete;

    // Удаляем у класса "MyUniquePTR" оператор присваивания
    MyUniquePTR& operator=(const MyUniquePTR&) = delete;

    // разрешаем перемещающий конструктор копирования
    MyUniquePTR(MyUniquePTR&& other) {
        std::swap(_rawPtr, other._rawPtr);
    }

    // разрешаем перемещающий оператор присваивания
    MyUniquePTR& operator=(MyUniquePTR&& other) {
        std::cout << "\"MyUniquePTR::operator=()\" working" << std::endl;

        if (this != &other) {
            delete _rawPtr;
            this->_rawPtr = other._rawPtr;

            // завершаем отъем ресурса у other
            other._rawPtr = nullptr;
        }

        return *this;
    }

    // Деструктор. Удаляет объект, на который ссылается умный указатель.
    ~MyUniquePTR() {
        delete _rawPtr;
    }

    // Возвращает указатель на константное значение, хранящийся внутри "MyUniquePTR"
    const T* GetRawPtr() const noexcept {
        return _rawPtr;
    }

    // Возвращает указатель и для чтения, и для записи
    T* GetRawPtr() noexcept {
        return _rawPtr;
    }

    // Прекращает владение объектом, адрес которого хранится в "_ptr"
    // и возвращает адрес объекта
    T* Release() noexcept {
        return std::exchange(_rawPtr, nullptr);
    }

    void MySwap(MyUniquePTR& other) {
        std::swap(_rawPtr, other._rawPtr);
    }

    const T* operator->() const {
        using namespace std::literals;

        if (!_rawPtr) {
            throw std::logic_error("Error: MyUniquePTR is null."s);
        }

        return _rawPtr;
    }

    T* operator->() {
        using namespace std::literals;

        if (!_rawPtr) {
            throw std::logic_error("Error: MyUniquePTR is null."s);
        }

        return _rawPtr;
    }

    const T& operator*() const {
        using namespace std::literals;
        if (!_rawPtr) {
            throw std::logic_error("Error: MyUniquePTR is null."s);
        }

        return *_rawPtr;
    }

    T& operator*() {
        using namespace std::literals;
        if (!_rawPtr) {
            throw std::logic_error("Error: MyUniquePTR is null."s);
        }

        return *_rawPtr;
    }

    // "explicit" используем в соответствии с Safe Bool Idiom
    explicit operator bool() const {
        return _rawPtr != nullptr;
    }
};