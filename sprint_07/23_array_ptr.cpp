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

#include <algorithm>  // std::swap
#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
   public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле _rawPtr должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        _rawPtr = (size) ? new Type[size] : nullptr;
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

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        std::swap(_rawPtr, other._rawPtr);
    }

   private:
    Type* _rawPtr = nullptr;
};

int main() {
    ArrayPtr<int> numbers(10);
    const auto& const_numbers = numbers;

    numbers[2] = 42;
    assert(const_numbers[2] == 42);
    assert(&const_numbers[2] == &numbers[2]);

    assert(numbers.Get() == &numbers[0]);

    ArrayPtr<int> numbers_2(5);
    numbers_2[2] = 43;

    numbers.swap(numbers_2);

    assert(numbers_2[2] == 42);
    assert(numbers[2] == 43);
}