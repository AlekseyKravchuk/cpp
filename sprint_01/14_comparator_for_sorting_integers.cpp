#include <algorithm>  // for usage of sort()
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    int number;
    vector<int> numbers;

    cin >> n;
    if (n == 0) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        cin >> number;
        numbers.push_back(number);
    }

    sort(numbers.begin(), numbers.end(),
         [](const int& a, const int& b) {
             if (a % 2 == 0 and b % 2 == 0) {
                 return a < b;
             } else if (a % 2 != 0 and b % 2 != 0) {
                 return a > b;
             } else if (a % 2 == 0 and b % 2 != 0) {
                 return true;
             } else {
                 return false;
             }
         });

    for (const auto& num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
