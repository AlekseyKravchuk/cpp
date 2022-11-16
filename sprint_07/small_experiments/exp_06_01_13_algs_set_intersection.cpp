#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <utility>
#include <vector>

using namespace std::literals;

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

int main() {
    std::set<int> a = {22, 2, 7, 120};
    std::set<int> b = {1, 3, 2, 6, 4, 7};

    // резервируем в результирующем (целевом, т.к. в том контейнере, КУДА мы будем складывать элементы) место с запасом
    std::vector<int> targetContainer(a.size() + b.size());

    // итератор "itPastTheEndNewRange" указывает на past-the-end нового диапазона, являющегося пересечением 2-х множеств
    auto itPastTheEndNewRange = std::set_intersection(begin(a), end(a),
                                                      begin(b), end(b),
                                                      begin(targetContainer));

    // смотрим, что получилось
    PrintRange(begin(targetContainer), end(targetContainer));

    // как и ожидалось, имеются пустые элементы - это обусловлено тем, что выделили место с запасом
    // 2, 7, 0, 0, 0, 0, 0, 0, 0, 0

    // Избавляемся от мусора, вызываем std::vector::erase
    targetContainer.erase(itPastTheEndNewRange, end(targetContainer));

    // смотрим, что получилось
    PrintRange(begin(targetContainer), end(targetContainer));
    std::cout << targetContainer.size() << std::endl;


    return 0;
}