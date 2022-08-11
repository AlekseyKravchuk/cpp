#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int myltiply(int x, int y) {
    return x*y;
}

int main() {
    // int size;
    // cin >> size;

    // vector<int> values;
    // for (int i = 0; i < size; ++i) {
    //     int value;
    //     cin >> value;
    //     values.push_back(value);
    // }

    vector<int> values = {2, 3, 10};
    int sum = accumulate(values.begin(), values.end(), 1, myltiply);
    cout << "sum = " << sum << endl;

    return 0;
}