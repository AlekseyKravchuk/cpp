#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> v = {1, 2, 3};
    int& first = v[0];

    cout << "first before push_back: " << first << endl;
    v.push_back(4);
    cout << "first AFTER push_back: " << first << endl;

    return 0;
}
