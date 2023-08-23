#include <set>

//  Элементы std::set нельзя модифицировать.

int main() {
    std::set<int> s{1, 2, 4, 5};
    auto it = s.begin();

    // Нельзя написать => и нельзя выполнить перемещение
    // ===> error: assignment of read-only location ‘it.std::_Rb_tree_const_iterator<int>::operator*()’
    *it = 50;  //error. value is readonly
    return 0;
}
