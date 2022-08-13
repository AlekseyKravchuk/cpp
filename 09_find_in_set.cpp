#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

void printSet(set<string> my_set) {
    for (auto it = my_set.begin(); it != my_set.end(); ++it) {
        if (next(it) != my_set.end()) {
            cout << *it << ", ";
        } else {
            cout << *it;
        }
    }
}

int main() {
    vector<string> v = {"my"s, "own"s, "cat"s, "big"s, "swallow"s, "bird"s};
    set<string> my_set(v.begin(), v.end());
    // const string str2search = "big"s;
    const string str2search = "ddd"s;

    if (my_set.find(str2search) != my_set.end()) {
        cout << "String \"" << str2search << "\" is found in set: {";
        printSet(my_set);
        cout << "}" << endl;
    } else {
        cout << "String \"" << str2search << "\" is NOT found in set: {";
        printSet(my_set);
        cout << "}" << endl;
    }

    return 0;
}
