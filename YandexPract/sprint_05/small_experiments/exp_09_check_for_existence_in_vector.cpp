#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<int> v = {4, 6, 1, 2, 8, 15, 3, 22, 5};
    auto checkResult = count(v.begin(), v.end(), 22);
    cout << boolalpha << checkResult << endl;
}