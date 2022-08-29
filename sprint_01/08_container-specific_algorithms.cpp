#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

const int SAMPLE_COUNT = 5;

vector<int> CountNamesLong(const set<string>& storage, const vector<string>& candidates) {
    vector<int> output;

    for (auto& name : candidates) {
        output.push_back(count(storage.begin(), storage.end(), name));
    }
    return output;
}

vector<int> CountNames(const set<string>& storage, const vector<string>& candidates) {
    vector<int> output;

    for (auto& name : candidates) {
        if (storage.find(name) != storage.end()) {
            output.push_back(1);
        } else {
            output.push_back(0);
        }
    }
    return output;
}

int main() {
    set<string> my_set;
    vector<string> v;
    string stra = ""s;
    string strb = ""s;

    for (int j = 0; j < 10000; ++j) {
        my_set.insert(stra);
        stra += "a"s;
        if (j % 2 == 0) {
            v.push_back(strb);
            strb += "b"s;
        } else {
            v.push_back(stra);
        }
    }
    cout << "Testing slow version" << endl;
    for (int i = 0; i < SAMPLE_COUNT; ++i) {
        auto begin = chrono::steady_clock::now();
        // CountNamesLong(my_set, v);
        CountNames(my_set, v);
        auto end = chrono::steady_clock::now();
        cout << "Time difference Long = "s
             << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]"s << endl;
    }
}