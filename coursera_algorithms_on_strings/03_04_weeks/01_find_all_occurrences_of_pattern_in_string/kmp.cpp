#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

vector<int> PrefixFunction(const string& text) {
    vector<int> s(text.size());
    int border = 0;

    for (size_t i = 1; i < text.size(); ++i) {
        while (border > 0 and text[i] != text[border]) {
            border = s[border-1];
        }

        border = (text[i] == text[border])
                 ? border + 1
                 : 0;

        s[i] = border;
    }

    return s;
}

vector<int> KnuthMorrisPratt(const string& text, const string& pattern) {
    string s = pattern + "$" + text;
    vector<int> pres = PrefixFunction(s);
    vector<int> positions;

    int patt_sz = static_cast<int>(pattern.size());
    int sz = static_cast<int>(s.size());

    for (int i = patt_sz + 1; i < sz; ++i) {
        if (pres[i] == patt_sz) {
            positions.push_back(i - 2 * patt_sz);
        }
    }

    return positions;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string pattern, text;
    cin >> pattern >> text;

    vector<int> positions = KnuthMorrisPratt(text, pattern);

    std::copy(positions.begin(), positions.end(),
              ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}

