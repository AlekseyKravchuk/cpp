#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "my_print.h"

using namespace std::literals;

// ЗАДАЧА: пусть требуется удалить некоторые элементы из контейнера на основании некоторого критерия.
// Как это сделать, используя ТОЛЬКО средства алгоритмов STL?
// Никак!
// С помощью стандартных алгоритмов нельзя укоротить или удлинить сам «диапазон» (range) -
// мы можем только изменять значения элементов в диапазоне.
// Таким образом, для действительного удаления элемента(ов) из контейнера мы можем организовать решение задачи в 2 этапа:

// 1. Найти способ переупорядочить значения в диапазоне так, чтобы «удаленные» элементы оказались где-то в предопределенном
// месте внутри контейнера, чтобы потом их можно было быстро удалить (используя другие средства, отличные от алгоритмов STL).
// 2. Собственно удалить по-честному нужные элементы и сократить размер контейнера.

int main() {
    // =================== С помощью std::stable_partition ===================
    {
        // требуется удалить все элементы со значением "3" из вектора
        std::vector<int> v = {1, 3, 3, 4, 6, 8, 3, 11, 7};
        PrintCollection(v, "Initial state of vector: ");
        std::cout << "Initial SIZE of vector: "s << v.size() << std::endl;
        std::cout << "Initial CAPACITY of vector: "s << v.capacity() << std::endl
                  << std::endl;
        auto sizeBefore = v.size();

        // Разбиваем вектор таким образом, чтобы все элементы, не равные "3" оказались в начале,
        // а все элементы, равные "3" оказались в конце.
        auto first_3 = std::stable_partition(v.begin(), v.end(),
                                             [](int value) {
                                                 return value != 3;
                                             });
        auto sizeAfterRearranging = v.size();

        // в результате применения "std::stable_partition"
        // в первую часть вектора попадут элементы, для которых унарный предикат возвращает "true",
        // а во вторую часть - те, для которых унарный предикат возвращает "false".
        // Алгоритм "std::stable_partition" возвращает итератор на ПЕРВЫЙ элемент во ВТОРОЙ группе элементов
        PrintCollection(v, "Vector after applying std::stable_partition: ");
        std::cout << "Позиция 1-го элемента во второй группе: "s << first_3 - v.begin() << std::endl
                  << std::endl;

        // выполняем нужные проверки
        assert(sizeBefore == sizeAfterRearranging);
        assert((v == std::vector /* <int> */ {1, 4, 6, 8, 11, 7, 3, 3, 3}));

        // теперь можно удалить "хвост" вектора
        // "std::vector::erase" удаляет диапазон элементов [first, last)
        v.erase(first_3, v.end());
        auto sizeAfterActualRemoving = v.size();
        // assert(sizeBefore == sizeAfterActualRemoving);
        PrintCollection(v, "Vector after applying std::vector::erase: ");
        std::cout << "New size of vector: "s << sizeAfterActualRemoving << std::endl;
        std::cout << "Resulting CAPACITY of vector: "s << v.capacity() << std::endl
                  << std::endl;
        // ПРИМЕЧАНИЕ
        // делается много ненужной работы (не забывайте, что stable_partition – один из алгоритмов STL,
        // которые выделяют буферы в динамической памяти!).
    }

    // =================== С помощью std::remove ===================
    {
        // требуется удалить все элементы со значением "3" из вектора
        std::vector<int> v = {1, 3, 3, 4, 6, 8, 3, 11, 7};
        PrintCollection(v, "Initial state of vector: ");
        std::cout << "Initial SIZE of vector: "s << v.size() << std::endl;
        std::cout << "Initial CAPACITY of vector: "s << v.capacity() << std::endl
                  << std::endl;

        int value2remove = 3;
        const auto new_end_it = std::remove(v.begin(), v.end(), value2remove);
        PrintCollection(v, "Vector after applying std::remove: ");
        std::cout << "SIZE of vector after std::remove: "s << v.size() << std::endl;
        std::cout << "CAPACITY of vector after std::remove: "s << v.capacity() << std::endl
                  << std::endl;

        v.erase(new_end_it, v.end());
        PrintCollection(v, "Vector after applying std::vector::erase: ");
        std::cout << "SIZE of vector after sstd::vector::erase: "s << v.size() << std::endl;
        std::cout << "CAPACITY of vector after std::vector::erase: "s << v.capacity() << std::endl;

        // В результате CAPACITY у нас осталась прежней.
        //  сократим эту емкость до реального размера вектора
        v.shrink_to_fit();
        std::cout << std::endl
                  << "CAPACITY of vector after std::vector::shrink_to_fit: "s
                  << v.capacity() << std::endl
                  << std::endl;

        // ПРИМЕЧАНИЕ!
        // Использование "std::vector::shrink_to_fit" может привести к тому,
        // что код обслуживания вектора выделит новый фрагмент памяти, который будет
        // иметь соответствующий размер, и переместит все элементы из старого фрагмента в новый
    }

    // =================== С помощью "erase-remove" idiom ===================
    {
        std::vector<int> v = {1, 3, 3, 4, 6, 8, 3, 11, 7};
        // v.erase(std::remove(v.begin(), v.end(), 3), v.end());

        // хотим удалить все нечетные элементы из контейнера
        const auto odd( [](auto val) { return val % 2 != 0;} );
        // const auto odd = [](auto val) { return val % 2 != 0;};
        v.erase(std::remove_if(v.begin(), v.end(), odd), v.end());

        PrintCollection(v, "Vector after applying erase-remove idiom: ");
    }

    return 0;
}
