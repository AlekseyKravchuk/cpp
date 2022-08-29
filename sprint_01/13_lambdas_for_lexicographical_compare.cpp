#include <algorithm>  // for usage of lexicographical_compare()
#include <iostream>
#include <string>
#include <vector>
// #include <sstream>

using namespace std;

bool CompIgnoreCase(char ch1, char ch2) {
    return tolower(ch1) < tolower(ch2);
}

int main() {
    // 8 quantity AMOUNT Selection Search 4ever music Mickey market

    // int n = 8;
    // vector<string> v = {"quantity"s, "AMOUNT"s, "Selection"s, "Search"s, "4ever"s, "music"s, "Mickey"s, "market"};

    int n;
    string word;
    vector<string> v;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> word;
        v.push_back(word);
    }

    // первый вариант использования sort +
    sort(v.begin(), v.end(), [](const string& a, const string& b) {
        return lexicographical_compare(a.begin(), a.end(),
                                       b.begin(), b.end(),
                                       CompIgnoreCase);
    });

    for (const auto& word : v) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}
