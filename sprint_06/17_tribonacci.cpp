#include <cstdint>
#include <deque>
#include <iostream>
#include <numeric>

using namespace std;

int64_t T(int i) {
    if (i <= 1) {
        return 0;
    }
    if (i == 2) {
        return 1;
    }

    std::deque<int64_t> q = {0, 0, 1};

    for (int n = 0; n < i - 2; ++n) {
        q.push_back(std::reduce(q.begin(), q.end()));
        q.pop_front();
    }

    return q.back();
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }
}