#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> v = {1, 4, 5};

    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        cout << *it << endl;
    }

    return 0;
}