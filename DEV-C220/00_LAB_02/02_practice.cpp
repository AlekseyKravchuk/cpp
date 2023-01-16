// Создайте класс, который должен предоставлять функциональность вектора с УНИКАЛЬНЫМИ значениями любого типа в заданном диапазоне.
// Например:
// В нашей структуре хранятся целые с уникальными значениями и данных задан диапазон [1,10].
//Пользователь хочет добавить в наш вектор {2,-1,3,33, 5,2}.
//В результате в нашем векторе должны храниться значения {2,3,5}

#include "02_practice.h"
#include "../my_print.h"

int main() {
    VectorWithUniques<int> v_uniques({1, 3, 9, 6, 4, 2, 3, 2, 3, 4, 11, 12, 3}, {2, 10});
    PrintCollection(v_uniques);

    std::cout << "current size of unique vector: "s << v_uniques.size() << std::endl;

    v_uniques.Append({7, 9, 5, 11});
    PrintCollection(v_uniques);

    // v_uniques.Remove({6, 2, 5});
    // PrintCollection(v_uniques);

    v_uniques.ChangeRange({2, 4});  // должно быть в результате: {3, 4, 2}
    PrintCollection(v_uniques);

    return 0;
}
