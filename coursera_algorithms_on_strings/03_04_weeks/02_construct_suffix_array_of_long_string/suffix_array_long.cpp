#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> SortCharacters(string s) {
    vector<int> order(s.size());
    unordered_map<char, int> count = {{'$', 0}, {'A', 0}, {'C', 0}, {'G', 0}, {'T', 0}};
    vector<char> symbols = {'$', 'A', 'C', 'G', 'T'};

    for (const char& ch: s) {
        ++count[ch];
    }

    for (int i = 1; i < 5; ++i) {
        count[symbols[i]] += count[symbols[i - 1]];
    }

    for (int j = s.size()-1; j >= 0; --j) {
        char ch = s[j];
        --count[ch];
        order[count[ch]] = j;
    }

    return order;
}

vector<int> ComputeCharClasses(const string& s, vector<int> order) {
    vector<int> char_classes(s.size());

    for (int i = 1; i < s.size(); ++i) {
        char_classes[order[i]] = (s[order[i]] == s[order[i-1]])
                                 ? char_classes[order[i]] = char_classes[order[i - 1]]
                                 : char_classes[order[i]] = char_classes[order[i - 1]] + 1;
    }

    return char_classes;
}

vector<int> SortDoubled(const string& s, int L, vector<int> old_order, vector<int> old_class) {
    vector<int> count(s.size()), new_order(s.size());

    for (size_t i = 0; i < s.size(); ++i) {
        ++count[old_class[i]];
    }

    for (int i = 1; i < s.size(); ++i) {
        count[i] += count[i - 1];
    }

    for (int j = s.size() - 1; j >= 0; --j) {
        int start = (old_order[j] - L + s.size()) % s.size();
        int ch_class = old_class[start];
        --count[ch_class];
        new_order[count[ch_class]] = start;
    }

    return new_order;
}

vector<int> UpdateClasses(vector<int> new_order, vector<int> old_class, int L) {
    int n = static_cast<int>(new_order.size());
    vector<int> new_class(n);

    for (int i = 1; i < n; ++i) {
        int cur = new_order[i];
        int mid = (cur + L) % n;
        int prev = new_order[i - 1];
        int mid_prev = (prev + L) % n;

        new_class[cur] = (old_class[cur] == old_class[prev] and old_class[mid] == old_class[mid_prev])
                        ? new_class[cur] = new_class[prev]
                        : new_class[cur] = new_class[prev] + 1;
    }

    return new_class;
}

vector<int> BuildSuffixArray(string s) {
    vector<int> order = SortCharacters(s);
    vector<int> ch_class = ComputeCharClasses(s, order);
    size_t L = 1;

    while (L < s.size()) {
        order = SortDoubled(s, L, order, ch_class);
        ch_class = UpdateClasses(order, ch_class, L);
        L = 2 * L;
    }

    return order;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string text;
    cin >> text;

    vector<int> suffix_array = BuildSuffixArray(text);

    std::copy(suffix_array.begin(), suffix_array.end(),
              ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}


