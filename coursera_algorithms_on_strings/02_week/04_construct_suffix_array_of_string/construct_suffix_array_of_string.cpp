#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    static const size_t MAX_TEXT_LENGTH = 10'001;
    string text;
    cin >> text;

    vector<pair<string, int>> suffix_array;
    suffix_array.reserve(MAX_TEXT_LENGTH);

    for (size_t i = 0; i < text.size(); i++) {
        suffix_array.emplace_back(text.substr(i), i);
    }

    std::sort(begin(suffix_array), end(suffix_array));

    for (const auto& suffix : suffix_array) {
        cout << suffix.second << " ";
    }

    cout << endl;

    return 0;
}
