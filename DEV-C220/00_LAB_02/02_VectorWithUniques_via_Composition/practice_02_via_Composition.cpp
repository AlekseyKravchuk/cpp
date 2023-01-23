// Создайте класс, который должен предоставлять функциональность вектора с УНИКАЛЬНЫМИ значениями любого типа в заданном диапазоне.
// Например:
// В нашей структуре хранятся целые с уникальными значениями и данных задан диапазон [1,10].
// Пользователь хочет добавить в наш вектор {2,-1,3,33, 5,2}.
// В результате в нашем векторе должны храниться значения {2,3,5}

#include "practice_02_via_Composition.h"

#include "../../my_print.h"

int main() {
    VectorWithUniques<int> v_uniques({-5, 1, 3, -2, 9, 6, 4, -4, 2, 3, 2, -7, 3, 4, -1, 11, 12, 3}, {-3, 7});
    PrintCollection(v_uniques);
    std::cout << "current size of unique vector: "s << v_uniques.size() << std::endl;

    v_uniques.Append({7, 9, -3, 5, 11, -7});
    PrintCollection(v_uniques, "After applying \"Append({7, 9, -3, 5, 11, -7})\": "s);

    v_uniques.Remove({6, 2, 5});
    PrintCollection(v_uniques, "After applying \"Remove({6, 2, 5})\": "s);

    v_uniques.ChangeRange({-1, 5});  // должно быть в результате: {3, 4, 2}
    PrintCollection(v_uniques, "After applying \"ChangeRange({-1, 5})\": "s);

    v_uniques.Sort([](int lhs, int rhs) { return lhs < rhs; });
    PrintCollection(v_uniques, "After sorting in Ascending order: ");

    v_uniques.Sort([](int lhs, int rhs) { return lhs > rhs; });
    PrintCollection(v_uniques, "After sorting in Descending order: ");

    v_uniques.Sort([](int lhs, int rhs) { return std::abs(lhs) < std::abs(rhs); });
    PrintCollection(v_uniques, "After sorting by Absolute values in Ascending order: ");

    v_uniques.Sort([](int lhs, int rhs) { return std::abs(lhs) > std::abs(rhs); });
    PrintCollection(v_uniques, "After sorting by Absolute values in Descending order: ");

    std::list<int> lst;
    std::copy(v_uniques.begin(), v_uniques.end(), std::inserter(lst, lst.begin()));
    PrintCollection(lst, "After copying collection from vector to list: "s);

    return 0;
}
