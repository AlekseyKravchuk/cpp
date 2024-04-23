#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

using TrickyPair = pair<unordered_map<char, int>, unordered_map<char, vector<int>>>;

TrickyPair Process(const string& text) {
    unordered_map<char, int> freq = {{'$', 0},
                                     {'A', 0},
                                     {'C', 0},
                                     {'G', 0},
                                     {'T', 0}};

    unordered_map<char, int> first_occur = {
            {'$', 0}
    };

    unordered_map<char, vector<int>> count;
    vector<char> elements = {'$', 'A', 'C', 'G', 'T'};

    for (const char& ch: text) {
        freq[ch]++;
    }

    for (int i = 1; i < 5; i++) {
        first_occur[elements[i]] = first_occur[elements[i-1]] + freq[elements[i-1]];
    }

    for (char ch: elements) {
        count[ch].assign(text.size() + 1, 0);
    }

    for (size_t i = 0; i < text.size(); i++) {
        unordered_map<char, int> dict = {{text[i], 1}};

        for (char ch: elements) {
            auto tmp = dict.count(ch)
                       ? dict[ch]
                       : 0;
            count[ch][i + 1] = count[ch][i] + tmp;
        }
    }

    return {first_occur, count};
}

int BW_Matching(const string& text,
                string pattern, unordered_map<char, int>& first_occur,
                unordered_map<char, vector<int>>& count) {
    int top = 0;
    int bottom = static_cast<int>(text.size()) - 1;

    while (top <= bottom) {
        if (not pattern.empty()) {
            char ch = pattern.back();
            pattern = pattern.substr(0, pattern.size() - 1);
            top = first_occur[ch] + count[ch][top];
            bottom = first_occur[ch] + count[ch][bottom+1] - 1;
        } else {
            return bottom - top + 1;
        }
    }

    return 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string text, pattern;
    int n;
    cin >> text;
    cin >> n;

    TrickyPair processed = Process(text);

    for (int i = 0; i < n; i++) {
        cin >> pattern;
        cout << BW_Matching(text, pattern, processed.first, processed.second) << " ";
    }

    cout << endl;

    return 0;
}
