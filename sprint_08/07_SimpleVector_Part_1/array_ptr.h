/*
Задание
Разработайте шаблонный класс ArrayPtr, играющий роль умного указателя на массив в динамической памяти. У него должен быть такой функционал:
удаление массива при разрушении умного указателя;
конструкторы: по умолчанию, из указателя на существующий массив, создающий новый массив заданного размера;
доступ к элементу массива по индексу;
запрет операций копирования и присваивания;
метод swap для обмена содержимым с другим объектом ArrayPtr;
метод Release, прекращающий владение массивом и возвращающий значение сырого указателя.
Ограничения
Сохраните сигнатуры всех публичных методов класса ArrayPtr неизменными, чтобы код скомпилировался без ошибок.
Что отправлять на проверку
В решении должен быть класс ArrayPtr с нужными для его компиляции директивами #include. Функция main учитываться при проверке не будет.
Как будет тестироваться ваш код
В качестве указателя, передаваемого в конструктор ArrayPtr, будет передаваться nullptr либо адрес массива в динамической памяти.
*/

#include <algorithm>  // std::swap, std::fill_n
#include <cassert>
#include <cstdlib>
#include <iostream>  // std::cerr

template <typename Type>
class ArrayPtr {
   public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле _rawPtr должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        try {
            _rawPtr = (size) ? new Type[size] : nullptr;
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

    // Создаёт в куче массив из size элементов типа Type, инициализированных значением "value"
    // Если size == 0, поле _rawPtr должно быть равно nullptr
    explicit ArrayPtr(size_t size, const Type& value) {
        try {
            _rawPtr = (size) ? new Type[size] : nullptr;
            std::fill_n(_rawPtr, size, value);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

    // Создаёт в куче массив из "values.size()" элементов, инициализированных с помощью std::initializer_list<Type>
    // Если "values.size == 0", поле _rawPtr должно быть равно nullptr
    explicit ArrayPtr(std::initializer_list<Type> values) {
        try {
            _rawPtr = (values.size()) ? new Type[values.size()] : nullptr;
            for (auto it = values.begin(); it != values.end(); ++it) {
                *(_rawPtr + (it - values.begin())) = *it;
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* rawPtr) noexcept {
        _rawPtr = rawPtr;
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        delete[] _rawPtr;
        _rawPtr = nullptr;
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        Type* ptrHolder = _rawPtr;
        _rawPtr = nullptr;
        return ptrHolder;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        return *(_rawPtr + index);
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        return *(_rawPtr + index);
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        return (_rawPtr != nullptr) ? true : false;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return _rawPtr;
    }

    void Set(Type* rawPtr) {
        _rawPtr = rawPtr;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        std::swap(_rawPtr, other._rawPtr);
    }

   private:
    Type* _rawPtr = nullptr;
};
