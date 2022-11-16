#pragma once

#include <vector>

template <typename T>
class PtrVector {
   public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Резервируем место в vector-е для хранения нужного количества элементов
        // Благодаря этому при push_back не будет выбрасываться исключение
        _items.reserve(other._items.size());

        try {
            for (auto p : other._items) {
                // Копируем объект, если указатель на него ненулевой
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение

                // Не выбросит исключение, т. к. в vector память уже зарезервирована
                _items.push_back(p_copy);
            }
        } catch (...) {
            // удаляем элементы в векторе и перевыбрасываем пойманное исключение
            DeleteItems();
            throw;
        }
    }

    PtrVector& operator=(const PtrVector& rhs) {
        if (this != &rhs) {
            // Реализация операции присваивания с помощью идиомы Copy-and-swap.
            // Если исключение будет выброшено, то на текущий объект оно не повлияет.
            // вызываем конструктор копирования
            auto rhsCopy(rhs);

            // rhsCopy содержит копию правого аргумента "rhs".
            // Обмениваемся с ним данными.
            swap(rhsCopy);

            // Теперь текущий объект содержит копию правого аргумента,
            // а rhsCopy - прежнее состояние текущего объекта, которое при выходе
            // из блока будет разрушено.
        }

        return *this;
    }

    // обменивает состояние текущего объекта с other без выбрасывания исключений
    void swap(PtrVector& other) noexcept {
        this->GetItems().swap(other.GetItems());
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        return _items;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        return _items;
    }

   private:
    void DeleteItems() noexcept {
        for (auto p : _items) {
            delete p;
        }
    }

    std::vector<T*> _items;
};