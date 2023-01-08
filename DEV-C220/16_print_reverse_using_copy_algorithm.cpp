#include <algorithm>
#include <iostream>
#include <iterator>  // std::begin(), std::end()
#include <ostream>
#include <vector>

// необходимо распечатать содержимое ЛЮБОГО контейнера (в том числе обычного статического массива)
// в обратном порядке с помощью алгоритма "std::copy"
template <typename Collection>
void PrintCollectionReverse(const Collection& c) {
    // для шаблоного класса "std::ostream_iterator<T>" необходимо указать тип "T"
    // и для этого используется "decltype(*std::begin(c) + *std::begin(c))"
    std::copy(std::crbegin(c),
              std::crend(c),
              std::ostream_iterator<decltype(*std::begin(c) + *std::begin(c))>(std::cout, " "));
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    PrintCollectionReverse(v);

    return 0;
}
