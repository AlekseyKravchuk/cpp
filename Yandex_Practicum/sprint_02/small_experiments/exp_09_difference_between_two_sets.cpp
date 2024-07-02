#include <iostream>
#include <set>
#include <algorithm>  // for std::set_difference()
#include <iterator>   // for std::inserter()

using namespace std;

template <class T>
void printSet(set<T> _set) {
    for (auto it = _set.begin(); it != _set.end(); ++it) {
        if (next(it) != _set.end()) {
            cout << *it << ", ";
        } else {
            cout << *it << endl;
        }
    }
}

int main() {
    set<int> set1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    set<int> set2 = {5, 6, 7};
    set<int> result;

    std::set_difference(set1.begin(), set1.end(),
                        set2.begin(), set2.end(),
                        std::inserter(result, result.end()));

    printSet(result);


    return 0;
}
