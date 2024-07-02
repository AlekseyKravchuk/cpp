#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

bool sortAscendingBySecondElm(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second != b.second) {      // if ages are NOT EQUAL
        return a.second > b.second;  // sort by name (by FIRST element of pair) alphabetically *(in ascending order) 
    } else {                         // ages are EQUAL => sort alphabetically (in ascending order) by name (1-st element of pair)
        return a.first < b.first;    // sort by age (by SECOND element of pair) in ascending order
    }
}

int main() {
    vector<pair<string, int>> person_info;
    int count;
    cin >> count;

    for (int i = 0; i < count; ++i) {
        string name;
        int age;
        cin >> name >> age;
        person_info.push_back({name, age});
    }

    // Sort alphabetically (ascending order) by first component of pair
    // sort(person_info.begin(), person_info.end());

    // Sort by second component of pair (i.e. by age) in ascending order
    sort(person_info.begin(), person_info.end(), sortAscendingBySecondElm);
    cout << "\n***************** Program output *****************" << endl;
    for (const auto& info_pair : person_info) {
        cout << info_pair.first << ": " << info_pair.second << endl;
    }

    return 0;
}

/*
====== Test input: ======
7
John 32
Helen 18
Andrey 41
Kate 31
Alex 18
Aleksey 35
Stephan 41
=========================
*/