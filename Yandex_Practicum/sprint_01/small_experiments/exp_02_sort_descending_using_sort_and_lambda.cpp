#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

void PrintVectorOfInt(vector<int>& v) {
    for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        if (next(it) != v.end()) {
            cout << *it << ", ";
        } else {
            cout << *it;
        }
    }
    cout << endl;
}

int main() {
    vector<int> v = {10, 15, 1, 4, 22, 7, 11, 9, 6, 3};
    sort(v.begin(), v.end(), [](int lhs, int rhs) {
        // return lhs < rhs;
        return lhs > rhs;
    });
    PrintVectorOfInt(v);

    return 0;
}
