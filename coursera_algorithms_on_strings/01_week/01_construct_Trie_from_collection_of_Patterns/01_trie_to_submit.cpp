#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

using edges = map<char, int>;
using trie = vector<edges>;

trie build_trie(vector<string>& patterns) {
    uint MAX_EDGES = 10'001;
    trie pref_tree(MAX_EDGES);
    uint node_cntr = 0;

    for (const auto& pattern : patterns) {
        uint src = 0;  // index of source vertex

        for (auto ch : pattern) {
            if (pref_tree[src].count(ch)) {
                src = pref_tree[src].at(ch);
                continue;
            } else {
                ++node_cntr;
                pref_tree[src].emplace(ch, node_cntr);
                src = node_cntr;
            }
        }
    }

    return pref_tree;
}

int main() {
    size_t n;
    std::cin >> n;
    vector<string> patterns;

    for (size_t i = 0; i < n; i++) {
        string s;
        std::cin >> s;
        patterns.push_back(s);
    }

    trie t = build_trie(patterns);

    for (size_t i = 0; i < t.size(); ++i) {
        for (const auto& j : t[i]) {
            std::cout << i << "->" << j.second << ":" << j.first << "\n";
        }
    }

    return 0;
}