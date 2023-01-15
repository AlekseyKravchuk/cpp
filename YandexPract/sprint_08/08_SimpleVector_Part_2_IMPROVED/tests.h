#pragma once

#include <cassert>
#include <stdexcept>

#include "simple_vector.h"

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
        v.Resize(old_size + 5);  // в результате: 0, 0, 0, 0,  0, 0, 0, 0
        v[3] = 42;               // в результате: 0, 0, 0, 42, 0, 0, 0, 0
        v.Resize(old_size);      // в результате: 0, 0, 0| 42, 0, 0, 0, 0
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

// ======================= Мои тесты =======================
// void MyTest() {
//     SimpleVector<int> v;
//     v.PushBack(42);
//     assert(v.GetSize() == 1);
//     assert(v.GetCapacity() >= v.GetSize());
//     assert(v[0] == 42);
// }

// void TestInitialization() {
//     // Инициализация конструктором по умолчанию
//     {
//         SimpleVector<int> v;
//         assert(v.GetSize() == 0u);
//         assert(v.IsEmpty());
//         assert(v.GetCapacity() == 0u);
//     }

//     // Инициализация вектора указанного размера
//     {
//         SimpleVector<int> v(5);
//         assert(v.GetSize() == 5u);
//         assert(v.GetCapacity() == 5u);
//         assert(!v.IsEmpty());
//         for (size_t i = 0; i < v.GetSize(); ++i) {
//             assert(v[i] == 0);
//         }
//     }

//     // Инициализация вектора, заполненного заданным значением
//     {
//         SimpleVector<int> v(3, 42);
//         assert(v.GetSize() == 3);
//         assert(v.GetCapacity() == 3);
//         for (size_t i = 0; i < v.GetSize(); ++i) {
//             assert(v[i] == 42);
//         }
//     }

//     // Инициализация вектора при помощи initializer_list
//     {
//         SimpleVector<int> v{1, 2, 3};
//         assert(v.GetSize() == 3);
//         assert(v.GetCapacity() == 3);
//         assert(v[2] == 3);
//     }
// }

// void TestCopyConstructor() {
//     {
//         SimpleVector<int> v1 = {1, 2, 3};
//         SimpleVector<int> v2 = v1;
//         PrintRange(v2.begin(), v2.end());
//     }
// }

// void TestAssignmentOperator() {
//     {
//         SimpleVector<int> v1 = {1, 2, 3};
//         SimpleVector<int> v2 = {5, 6, 7};
//         v2 = v1;
//         PrintRange(v2.begin(), v2.end());
//     }
// }

// void TestInsert() {
//     {
//         SimpleVector<int> v1 = {1, 2, 3, 4};
//         auto it = v1.begin() + 1;
//         v1.Insert(it, 88);

//         std::cout << "After insertion: ";
//         PrintRange(v1.begin(), v1.end());

//         auto it2 = v1.begin();
//         v1.Insert(it2, 88);
//         PrintRange(v1.begin(), v1.end());
//     }
// }

// void TestPushBack() {
//     {
//         SimpleVector<int> v1 = {1, 2, 3, 4};
//         v1.PushBack(88);
//         std::cout << "After insertion: ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "_size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;
//         std::cout << "============================================" << std::endl;
//     }

//     {
//         std::cout << "Test PushBack into empty SimpleVector:\n";
//         SimpleVector<int> v1;
//         v1.PushBack(88);
//         std::cout << "After insertion: ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "_size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;
//     }
// }

// void TestResize() {
//     {
//         std::cout << "===============Simple case===============" << std::endl;
//         SimpleVector<int> v1 = {1, 2, 3, 4};
//         std::cout << "Initial dimensions: _size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;

//         v1.Resize(9);

//         std::cout << "After Resizing: ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "_size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;
//     }

//     {
//         std::cout << "=============== Initial vector is empty ===============" << std::endl;
//         SimpleVector<int> v1;
//         std::cout << "Initial dimensions: _size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;

//         v1.Resize(5);

//         std::cout << "After Resizing: ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "_size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;
//     }

//     {
//         std::cout << "=============== Filling with default values ===============" << std::endl;
//         SimpleVector<int> v1 = {1, 2, 3};
//         std::cout << "Initial dimensions: _size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;

//         v1.Resize(8);
//         std::cout << "After Resizing 3->8: ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "_size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;

//         v1.Resize(2);
//         std::cout << "After Resizing 3->8->2: ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "_size = " << v1.GetSize() << "; _capacity = " << v1.GetCapacity() << std::endl;
//     }
// }

// // test for operator==
// void TestOperatorEqualEqual() {
//     {
//         std::cout << "=============== Test operator== ===============" << std::endl;
//         SimpleVector<int> v1 = {1, 2, 3, 4};
//         SimpleVector<int> v2 = {1, 2, 3, 5};
//         std::cout << "v1 = ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "v2 = ";
//         PrintRange(v2.begin(), v2.end());
//         std::cout << std::boolalpha << (v1 == v2) << std::endl;
//     }
// }

// // test for operator<
// void TestOperatorLessThan() {
//     {
//         std::cout << "=============== Test operator< ===============" << std::endl;
//         SimpleVector<int> v1 = {1, 2, 3, 4};
//         SimpleVector<int> v2 = {1, 2, 3, 5};
//         std::cout << "v1 = ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "v2 = ";
//         PrintRange(v2.begin(), v2.end());
//         std::cout << std::boolalpha << (v1 < v2) << std::endl;
//     }

//     {
//         std::cout << "=============== Test operator< ===============" << std::endl;
//         SimpleVector<int> v1 = {1, 2, 2, 5, 4};
//         SimpleVector<int> v2 = {1, 2, 3, 5};
//         std::cout << "v1 = ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "v2 = ";
//         PrintRange(v2.begin(), v2.end());
//         std::cout << std::boolalpha << (v1 < v2) << std::endl;
//     }

//     {
//         std::cout << "=============== Test operator< ===============" << std::endl;
//         SimpleVector<char> v1 = {'a', 'b', 'b'};
//         SimpleVector<char> v2 = {'a', 'b', 'v'};
//         std::cout << "v1 = ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "v2 = ";
//         PrintRange(v2.begin(), v2.end());
//         std::cout << std::boolalpha << (v1 < v2) << std::endl;
//     }
// }

// void TestOperatorLessThanOrEqual() {
//     {
//         std::cout << "=============== Test operator<= ===============" << std::endl;
//         SimpleVector<char> v1 = {'a', 'b', 'b'};
//         SimpleVector<char> v2 = {'a', 'b', 'v'};
//         std::cout << "v1 = ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "v2 = ";
//         PrintRange(v2.begin(), v2.end());
//         std::cout << std::boolalpha << (v1 <= v2) << std::endl;
//     }

//     {
//         std::cout << "=============== Test operator<= ===============" << std::endl;
//         SimpleVector<char> v1 = {'a', 'b', 'v'};
//         SimpleVector<char> v2 = {'a', 'b', 'v'};
//         std::cout << "v1 = ";
//         PrintRange(v1.begin(), v1.end());
//         std::cout << "v2 = ";
//         PrintRange(v2.begin(), v2.end());
//         std::cout << std::boolalpha << (v1 <= v2) << std::endl;
//     }
// }