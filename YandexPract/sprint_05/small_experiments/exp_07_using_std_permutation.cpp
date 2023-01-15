#include <algorithm>
#include <iostream>
#include <string>

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

int main() {
    // std::string s = "aba";
    std::vector<int> numbers = {3, 4, 1, 2};
    std::sort(numbers.begin(), numbers.end());
    do {
        PrintContainer(numbers);
    } while (std::next_permutation(numbers.begin(), numbers.end()));
}