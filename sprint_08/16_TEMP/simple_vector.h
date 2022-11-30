#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <utility>

#include "array_ptr.h"

using namespace std;

// Класс-обертка для различия версий конструкторов с параметрами size и reserve (оба типа size_t)
class ReserveProxyObj {
    size_t capacity = 0;

   public:
    ReserveProxyObj() = delete;

    explicit ReserveProxyObj(size_t new_capacity) : capacity(new_capacity) {}

    size_t GetCapacity() { return capacity; }
};

// Функция, создающая прокси-объект для передачи его в конструктор SimpleVector
ReserveProxyObj Reserve(size_t newCapacity) {
    return ReserveProxyObj(newCapacity);
}

template <typename Type>
class SimpleVector {
    ArrayPtr<Type> arr_;

    size_t size_ = 0;      // Количество элементов в векторе
    size_t capacity_ = 0;  // Выделено памяти в векторе

    // ========== Я не понимаю, что вы от меня хотите ============
    // описание ниже
    void ChangeSize(const size_t new_size) {
        SimpleVector new_arr(new_size);
        size_t prev_size = size_;
        std::move(begin(), end(), new_arr.begin());
        swap(std::move(new_arr));
        size_ = prev_size;
    }

    // void ChangeSize(const size_t new_size) {
    //     // выделили память без инициализации
    //     auto new_arr ArrayPtr(new_size);

    //     size_t prev_size = size_;

    //     // перемещаем данные в новое место
    //     std::move(begin(), end(), new_arr.Get());
    //     // >>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<
    //     // а вот как для SimpleVector::swap из ArrayPtr<Type> сделать SimpleVector<Type>, я не знаю
    //     swap(std::move(?????));
    //     size_ = prev_size;
    // }

   public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : SimpleVector(size, std::move(Type{})) {
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : arr_(size),
                                                   size_(size),
                                                   capacity_(size) {
        std::fill(begin(), end(), value);
    }

    // этот конструктор ТОЧНО нужен
    // И нужен он ровно потому, что без него программа не компилируется
    // Сделал, как смог. Ждать ответа НАСТАВНИКА по 24 часа на любой вопрос не представляется возможным!
    SimpleVector(size_t size, Type&& value) : arr_(size),
                                              size_(size),
                                              capacity_(size) {
        std::generate(arr_.Get(), arr_.Get() + size_, []() {
            return Type{};
        });
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : arr_(init.size()),
                                                     size_(init.size()),
                                                     capacity_(init.size()) {
        std::move(init.begin(), init.end(), arr_.Get());
    }

    SimpleVector(const SimpleVector& other) {
        // Сначала надо удостовериться, что текущий массив пуст (громоздкости от проверки не добавилось)
        assert(size_ == 0 && arr_.Get() == nullptr);

        // создаем временный вектор с такой же "capacity_", как и у "other"
        // используем класс-обертку ReserveProxyObj для непосредственного задания "capacity_", а не "size_"
        SimpleVector tmp(Reserve(other.GetCapacity()));

        // копируем во временный массив элементы из "other"
        std::copy(other.begin(), other.end(), tmp.begin());

        // После того как элементы скопированы, обмениваем все данные текущего массива и tmp
        swap(tmp);
    }

    // move-constructor
    SimpleVector(SimpleVector&& other) noexcept : arr_(other.size_) {
        if (this != &other) {
            swap(other);
        }
    }

    SimpleVector(ReserveProxyObj obj) : arr_(obj.GetCapacity()),
                                        size_(0),
                                        capacity_(obj.GetCapacity()) {
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        // 1) создаем временный массив такой же емкости, как и "rhs"
        SimpleVector<Type> tmp(Reserve(rhs.GetCapacity()));

        // 2) копируем в него элементы из rhs
        std::copy(rhs.begin(), rhs.end(), tmp.begin());

        // 3) Обмениваем содержимое временной копии и текущего объекта.
        // При выходе из "оператора =" временный объект будет разрушен, освободив память от предыдущего содержимого списка.
        swap(tmp);
    }

    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        swap(std::move(rhs));

        return *this;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return *(begin() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return *(begin() + index);
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept { return size_; }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept { return capacity_; }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept { return (size_ == 0); }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_)
            throw std::out_of_range("Invalid index");
        return *(begin() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_)
            throw std::out_of_range("Invalid index");
        return *(begin() + index);
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept { size_ = 0; }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
            return;
        }

        if (new_size <= capacity_) {
            // Забиваем стандартным значением добавленные после size_ элементы
            std::generate(begin() + size_, begin() + new_size, []() {
                return Type{};
            });
        } else {
            ChangeSize(new_size);
            std::generate(begin() + size_, begin() + new_size, []() {
                return Type{};
            });
        }

        size_ = new_size;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (size_ == capacity_) {
            size_ != 0 ? ChangeSize(2 * size_) : ChangeSize(1);  // Если вектор пустой, то изменяем вместимость на 1
        }
        arr_[size_] = item;
        size_++;
    }

    void PushBack(Type&& item) {
        if (size_ == capacity_) {
            if (size_) {
                ChangeSize(2 * size_);
            } else {
                ChangeSize(1);
            }
        }
        
        arr_[size_] = std::move(item);
        item = std::move(Type{});
        size_++;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        auto index = std::distance(cbegin(), pos);
        if (size_ == capacity_)
            size_ != 0 ? ChangeSize(2 * size_) : ChangeSize(1);  // Если вектор пустой, то изменяем вместимость на 1
        auto it = begin() + index;
        std::move_backward(it, end(), end() + 1);  // Сдвигаем элменеты на один вправо
        arr_[index] = std::move(value);
        ++size_;
        return Iterator(begin() + index);
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        auto index = std::distance(cbegin(), pos);
        if (size_ == capacity_) {
            size_ != 0 ? ChangeSize(2 * size_) : ChangeSize(1);  // Если вектор пустой, то изменяем вместимость на 1
        }

        auto it = begin() + index;
        std::move_backward(it, end(), end() + 1);
        arr_[index] = std::move(value);
        ++size_;
        value = std::move(Type{});
        return Iterator(begin() + index);
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (IsEmpty()) {
            std::cerr << "PopBack: SimpleVector is empty. PopBack() is impossible." << std::endl;
        };
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos < end());
        auto index = std::distance(cbegin(), pos);
        auto it = begin() + index;
        std::move((it + 1), end(), it);
        --size_;
        return Iterator(pos);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        arr_.swap(other.arr_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void swap(SimpleVector&& other) noexcept {
        arr_.swap(other.arr_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Увеличивает емкость вектора
    void Reserve(size_t new_capacity) {
        if (capacity_ < new_capacity)
            ChangeSize(new_capacity);
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept { return arr_.Get(); }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept { return arr_.Get() + size_; }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept { return ConstIterator(arr_.Get()); }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept { return ConstIterator(arr_.Get() + size_); }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept { return begin(); }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept { return end(); }
};

template <typename Type>
bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs > lhs);
}
