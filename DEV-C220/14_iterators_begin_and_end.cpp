#include <vector>

#include "my_print.h"

int main() {
    std::vector<int> v = {1, 2, 3};
    auto it1 = v.begin();   // "it1" имеет тип "std::vector<int>::iterator"
    auto it2 = v.cbegin();  // "it2" имеет тип "std::vector<int>::const_iterator"

    *it1 = 200;
    PrintCollection(v);

    // *it2 = 555;  // ОШИБКА!!!
    // PrintCollection(v);

    return 0;
}
