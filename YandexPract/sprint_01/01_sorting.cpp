#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool sortBySecondElm(const pair<int, string>& a, const pair<int, string>& b) {
    if (a.second == b.second) {
        return a.first < b.first;
    } else {
        return a.second < b.second;
    }
}

int main() {
    vector<int> numbers = {1, 5, 12, 3, 7};
    vector<string> continents = {"Eurasia"s, "Antarctica"s, "North America"s, "Australia"s};
    vector<pair<int, string>> numbers_with_names = {{2, "Two"s}, {1, "First"s}, {6, "Eight"s}, {1, "One"s}, {3, "Three"s}, {8, "Eight"s}};

    sort(numbers.begin(), numbers.end());
    sort(continents.begin(), continents.end());
    sort(numbers_with_names.begin(), numbers_with_names.end(), sortBySecondElm);

    // reverse(numbers.begin(), numbers.end());
    // reverse(continents.begin(), continents.end());

    for (const auto& element : numbers) {
        cout << element << endl;
    }
    cout << endl;

    for (const auto& element : continents) {
        cout << element << endl;
    }
    cout << endl;

    for (const auto& element : numbers_with_names) {
        cout << element.first << " " << element.second << endl;
    }
}