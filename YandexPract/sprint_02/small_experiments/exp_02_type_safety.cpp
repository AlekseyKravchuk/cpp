#include <iostream>
#include <vector>
using namespace std;
int main() {
    vector<int> v = {4, 5};

    for (int i = 0; i < v.size(); ++i) {
        cout << i << ": "s << v[i] << endl;
    }
}