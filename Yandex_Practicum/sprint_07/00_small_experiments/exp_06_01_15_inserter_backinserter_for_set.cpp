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

    // создаем контейнер, КУДА мы будем складывать элементы
    std::set<int> resIntersect;

    // результат нужно сохранять в итератор, который будет выполнять операцию "insert"
    // итератор std::inserter позволяет вставлять во множество
    std::set_intersection(a.begin(), a.end(),
                          b.begin(), b.end(),
                          std::inserter(resIntersect /* куда вставлять */, resIntersect.end()) /* перед каким итератором вставлять */);
    
    PrintRange(resIntersect.begin(), resIntersect.end());

    return 0;
}