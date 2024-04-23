#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<int> LastToFirst(const string& text) {
    unordered_map<char, int> counts = {{'$', 0},
                                       {'A', 0},
                                       {'C', 0},
                                       {'G', 0},
                                       {'T', 0}};
    unordered_map<char, int> position;
    vector<int> answer(text.size());
    int temp = -1;

    for (const char& ch: text) {
        counts[ch]++;
    }

    for (char t: "$ACGT") {
        temp += counts[t];
        position[t] = temp;
    }

    for (int i = static_cast<int>(text.size()) - 1; i >= 0; --i) {
        answer[i] = position[text[i]];
        --position[text[i]];
    }

    return answer;
}

string InvertBurrowsWheelerTransform(const string& text) {
    string answer = "$";
    int pos = 0;
    vector<int> ltf = LastToFirst(text);

    for (int i = 0; i < static_cast<int>(text.size()) - 1; i++) {
        answer += text[pos];
        pos = ltf[pos];
    }

    reverse(begin(answer), end(answer));

    return answer;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string text;
    cin >> text;

    cout << InvertBurrowsWheelerTransform(text) << endl;

    return 0;
}