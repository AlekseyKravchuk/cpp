#include <iostream>
#include <vector>

using namespace std;

// vector used to store elements
vector<int> elms;

// vector used to store current comb of length k
vector<int> comb;

void print_current_comb(const vector<int>& v) {
    static int count = 1;
    cout << "combination no " << (count) << ": [ ";
    count++;

    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }

    cout << "] " << endl;
}


void go(int offset, int k) {
    static int n = elms.size();

    if (k == 0) {
        print_current_comb(comb);
        return;
    }

    for (int i = offset; i <= n - k; i++) {
        comb.push_back(elms[i]);
        go(i+1, k-1);
        comb.pop_back();
    }
}

int main() {
    int n = 5;
    int k = 3;

    // populate vector elms with values 1, 2, ..., n
    for (int i = 0; i < n; i++) {
        elms.push_back(i+1);
    }

    go(0, k);

    return 0;
}
