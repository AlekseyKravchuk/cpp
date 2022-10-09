// C++ program to illustrate
// std :: iota
#include <algorithm>
#include <iostream>  // std::cout
#include <numeric>   // std::iota
#include <vector>

using namespace std;

template <class Container>
void PrintContainer(Container& container) {
    bool isFirst = true;

    for (auto& elm : container) {
        if (isFirst) {
            cout << elm;
            isFirst = false;
        } else {
            cout << " " << elm;
        }
    }

    cout << endl;
}

// Driver code
int main() {
    std::vector<int> numbers = {4, 2, 3, 1};
    PrintContainer(numbers);

    auto firstIt = numbers.begin();
    auto lastIt = numbers.end();

    sort(firstIt, lastIt);
    PrintContainer(numbers);

    cout << *firstIt << endl;
    cout << endl;

    do {
        std::cout << *firstIt << '\n';
    } while (std::next_permutation(firstIt, lastIt));

    return 0;
}
