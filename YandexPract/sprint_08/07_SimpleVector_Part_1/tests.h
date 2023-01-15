#pragma once

#include <cassert>
#include <stdexcept>

#include "simple_vector.h"

template <typename Iterator>
void PrintRange(Iterator range_begin,
                Iterator range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        if (std::next(it) != range_end) {
            std::cout << *it << ", "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

// У функции, объявленной со спецификатором inline, может быть несколько
// идентичных определений в разных единицах трансляции.
// Обычно inline помечают функции, чьё тело находится в заголовочном файле,
// чтобы при подключении этого файла из разных единиц трансляции не возникало ошибок компоновки
inline void Test1() {
    // Инициализация конструктором по умолчанию
    {
        SimpleVector<int> v;
        assert(v.GetSize() == 0u);
        assert(v.IsEmpty());
        assert(v.GetCapacity() == 0u);
    }

    // Инициализация вектора указанного размера
    {
        SimpleVector<int> v(5);
        assert(v.GetSize() == 5u);
        assert(v.GetCapacity() == 5u);
        assert(!v.IsEmpty());
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 0);
        }
    }

    // Инициализация вектора, заполненного заданным значением
    {
        SimpleVector<int> v(3, 42);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 42);
        }
    }

    // Инициализация вектора при помощи initializer_list
    {
        SimpleVector<int> v{1, 2, 3};
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        assert(v[2] == 3);
    }

    // Доступ к элементам при помощи At
    {
        SimpleVector<int> v(3);
        assert(&v.At(2) == &v[2]);
        try {
            v.At(3);
            assert(false);  // Ожидается выбрасывание исключения
        } catch (const std::out_of_range&) {
        } catch (...) {
            assert(false);  // Не ожидается исключение, отличное от out_of_range
        }
    }

    // Очистка вектора
    {
        SimpleVector<int> v(10);
        const size_t old_capacity = v.GetCapacity();
        v.Clear();
        assert(v.GetSize() == 0);
        assert(v.GetCapacity() == old_capacity);
    }

    // Изменение размера
    {
        SimpleVector<int> v(3);
        v[2] = 17;
        v.Resize(7);
        assert(v.GetSize() == 7);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[2] == 17);
        assert(v[3] == 0);
    }
    {
        SimpleVector<int> v(3);
        v[0] = 42;
        v[1] = 55;
        const size_t old_capacity = v.GetCapacity();
        v.Resize(2);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
        assert(v[0] == 42);
        assert(v[1] == 55);
    }

    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.Resize(old_size + 5);
        v[3] = 42;
        v.Resize(old_size);
        v.Resize(old_size + 2);
        assert(v[3] == 0);
    }

    // Итерирование по SimpleVector
    {
        // Пустой вектор
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        // Непустой вектор
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.GetSize());
        }
    }
}

void MyInitializationsTests() {
    {
        // Инициализация конструктором по умолчанию
        SimpleVector<int> v;
        std::cout << "Инициализация конструктором по умолчанию" << std::endl;
    }

    {
        // Инициализация вектора указанного размера
        SimpleVector<int> v(5);
        std::cout << "Инициализация вектора указанного размера" << std::endl;
    }

    {
        // Инициализация вектора, заполненного заданным значением
        SimpleVector<int> v(3, 42);
        std::cout << "Инициализация вектора, заполненного заданным значением" << std::endl;
    }

    {
        // Инициализация вектора при помощи initializer_list
        SimpleVector<int> v{1, 2, 3};
        std::cout << "Инициализация вектора при помощи initializer_list" << std::endl;
    }
}

void MyResizingTest() {
    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.Resize(old_size + 5);
        v[3] = 42;
        v.Resize(old_size);
        v.Resize(old_size + 2);
        std::cout << "v[3] = "s << v[3] << std::endl;
        assert(v[3] == 0);
    }
}

void MyIteratingOverContainverTest() {
    {
        // v.size() == 7;
        // SimpleVector<int> v = {20, 15, 22, 4, 7, 11, 19};
        SimpleVector<int> v(10);
        PrintRange(v.begin(), v.end());

        for (auto it = v.begin(); it != v.end(); ++it) {
            std::cout << *it << " "s;
        }
        std::cout << std::endl;

        std::cout << "v[6] = "s << v[6] << std::endl;
        std::cout << "v.At(6) = "s << v.At(6) << std::endl;

        std::cout << "\n=============================\n";
        v.Resize(3);
        PrintRange(v.begin(), v.end());

        for (auto it = v.begin(); it != v.end(); ++it) {
            std::cout << *it << " "s;
        }
        std::cout << std::endl;

        std::cout << "v[2] = "s << v[2] << std::endl;
        std::cout << "v.At(2) = "s << v.At(2) << std::endl;

        std::cout << "\n=============================\n";
        v.Resize(5);
        PrintRange(v.begin(), v.end());

        for (auto it = v.begin(); it != v.end(); ++it) {
            std::cout << *it << " "s;
        }
        std::cout << std::endl;

        std::cout << "v[5] = "s << v[5] << std::endl;
        std::cout << "v.At(5) = "s << v.At(5) << std::endl;

        // std::cout << "\n=============================\n";
        // v.Resize(2);
        // PrintRange(v.begin(), v.end());

        // for (auto it = v.begin(); it != v.end(); ++it) {
        //     std::cout << *it << " "s;
        // }
    }
}