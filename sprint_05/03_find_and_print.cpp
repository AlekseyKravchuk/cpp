#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Yandex version
template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <class ContainerType, class ElementType>
void FindAndPrint(const ContainerType& collection, const ElementType& elementToSearchFor) {
    auto it_dividing = find(collection.begin(), collection.end(), elementToSearchFor);
    PrintRange(collection.begin(), it_dividing);
    PrintRange(it_dividing, collection.end());
}


int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    cout << "Test1"s << endl;
    FindAndPrint(test, 3);
    cout << "Test2"s << endl;
    FindAndPrint(test, 0); // элемента 0 нет в контейнере
    cout << "End of tests"s << endl;
} 