#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    vector<int> v{8};
    v.reserve(3);
    size_t i = 2;
    cout << v[i] << endl;
    return 0;
}
