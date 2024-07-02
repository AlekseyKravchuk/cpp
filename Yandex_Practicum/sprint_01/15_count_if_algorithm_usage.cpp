#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int threshold;
    cin >> threshold;

    const vector<int> v = {1, 3, 5, 2, 6, 7, 10, 2, 3};
    cout << count_if(begin(v), end(v), [threshold](int x) { return x > threshold; }) << endl;

    return 0;
}
