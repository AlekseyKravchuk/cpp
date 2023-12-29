#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int const Letters = 4;
int const NA = -1;

struct Node {
    int next[Letters];

    Node() {
        std::fill(next, next + Letters, NA);
    }

    bool isLeaf() const {
        return std::all_of(next,
                           next + Letters,
                           [](int value) {
                               return value == NA;
                           });
    }
};

int letterToIndex(char letter) {
    switch (letter) {
    case 'A':
        return 0;
        break;
    case 'C':
        return 1;
        break;
    case 'G':
        return 2;
        break;
    case 'T':
        return 3;
        break;
    default:
        assert(false);
        return -1;
    }
}

vector<Node> build_trie(const vector<string>& patterns) {
    vector<Node> trie(1);
    for (const auto& pattern : patterns) {
        size_t src = 0;  // index of source vertex

        for (auto ch : pattern) {
            size_t i = letterToIndex(ch);
            size_t dst = trie[src].next[i];  // index of destination vertex

            if (dst == NA) {
                dst = trie.size();
                trie.push_back(Node());
                trie[src].next[i] = dst;
                src = dst;
            }
            
            src = dst;
        }
    }

    return trie;
}

vector<int> solve(const string& text, int n, const vector<string>& patterns) {
    vector<int> result;

    // write your code here

    return result;
}

int main(void) {
    string text;
    cin >> text;

    int n;
    cin >> n;

    vector<string> patterns(n);
    for (int i = 0; i < n; i++) {
        cin >> patterns[i];
    }

    vector<int> ans;
    ans = solve(text, n, patterns);

    for (int i = 0; i < (int)ans.size(); i++) {
        cout << ans[i];
        if (i + 1 < (int)ans.size()) {
            cout << " ";
        } else {
            cout << endl;
        }
    }

    return 0;
}
