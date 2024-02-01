#include <algorithm>
#include <iostream>
#include <set>
#include <optional>
#include <string>
#include <unordered_map>

using namespace std;

int main() {
    set<int> numbers = {3, 7, 5, 6, 20, 4, 22, 17, 9};

    // auto it = std::find(cbegin(numbers), cend(numbers), 4);
    auto it = std::find(begin(numbers), end(numbers), 4);

    // константность в С++ позволяет поддерживать инварианты в классах и объектах
    const int& set_elm_ref = *it;  // compiles fine

    // int& set_elm_ref = *it;  // error: binding reference of type ‘int&’ to ‘const int’ discards qualifiers
    // *it = 21;

    for (auto x : numbers) {
        cout << x << ' ';
    }

    return 0;
}
