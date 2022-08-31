#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    std::vector<int> v;
    // Initialising vector
    // vector = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; vector.size() = 10;
    for (int i = 0; i < 10; i++) {
        v.push_back(i * 10);
    }

    int index = 5;

    // // print all elements of vector starting from {index}th+1 element: {50, 60, 70, 80, 90}
    // for (auto it = next(v.begin(), index); it != v.end(); ++it) {
    //     cout << *it << " ";
    // }
    // cout << endl;

    auto it_start = v.begin() + 2;
    auto it_end = v.rbegin() + 3;
    for (it_start; it_start != it_end.base(); ++it_start) {
        cout << *it_start << " ";
    }
    cout << endl;

    // for (auto it = v.begin(); it < v.end(); advance(it, 1)) {
    //      cout << *it << " ";
    // }
    // cout << endl;

    return 0;
}
