#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <class InputIterator>
void PrintRange(InputIterator first, InputIterator last) {
    while (first != last) {
        if (next(first) != last) {
            cout << *first << " "s;
        } else {
            cout << *first << endl;
        }

        ++first;
    }
}

/* // Yandex version
template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
} */


int main() {
    cout << "Test1"s << endl;
    set<int> test1 = {1, 1, 1, 2, 3, 4, 5, 5};
    PrintRange(test1.begin(), test1.end());
    cout << "Test2"s << endl;
    vector<int> test2 = {};  // пустой контейнер
    PrintRange(test2.begin(), test2.end());
    cout << "End of tests"s << endl;
}