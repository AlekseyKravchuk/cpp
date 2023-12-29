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

    for (const string& pattern : patterns) {
        size_t src = 0;

        for (char ch : pattern) {
            int i = letterToIndex(ch);

            if (trie[src].next[i] == NA) {
                trie[src].next[i] = static_cast<int>(trie.size());
                trie.push_back(Node{});
            }
            src = trie[src].next[i];
        }
    }

    return trie;
}

vector<int> solve(const string& text, int n, const vector<string>& patterns) {
    assert(n == static_cast<int>(patterns.size()));
    vector<int> result;
    vector<Node> trie = build_trie(patterns);

    for (size_t start = 0; start < text.size(); ++start) {
        size_t src = 0;  // всегда начинаем проходить префиксное дерево шаблонов от корня; "src" - индекс в векторе "trie"
        size_t i = 0;    // индекс в массиве "next" структуры "Node"

        // "приложить" к тексту с позиции "pos" префиксное дерево из шаблонов поиска ("trie")
        size_t pos = start;
        while (pos < text.size()) {
            i = letterToIndex(text[pos]);
            if (trie[src].next[i] == NA) {
                break;
            }

            src = trie[src].next[i];
            ++pos;
        }

        if (trie[src].isLeaf()) {
            result.push_back(static_cast<int>(start));
        }
    }

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
