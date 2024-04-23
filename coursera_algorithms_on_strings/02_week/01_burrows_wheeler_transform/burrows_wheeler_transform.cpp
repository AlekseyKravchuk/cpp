#include <string>
#include <vector>
#include <algorithm>  // std::sort
#include <numeric>    // std::accumulate
#include <iostream>

using namespace std;

string burrows_wheeler_transform(const string& text) {
    vector<string> m{text};

    for (size_t i = 1; i < text.size(); ++i) {
        m.push_back(text.substr(i) + text.substr(0, i));
    }

    std::sort(begin(m), end(m));

    return accumulate(begin(m),
                      end(m),
                      string(),
                      [](const string& bwt, string m_row) {
                          return bwt + m_row.back();
                      });
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string text;
    cin >> text;

    cout << burrows_wheeler_transform(text) << endl;

    return 0;
}