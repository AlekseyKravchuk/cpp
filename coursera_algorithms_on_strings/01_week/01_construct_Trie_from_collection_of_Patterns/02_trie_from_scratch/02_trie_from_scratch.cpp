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
    size_t MAX_EDGES = 10'000;
    trie t(MAX_EDGES);
    size_t node_cntr = 0;
    
    for (const string& pattern : patterns) {
        // indices of source vertice of current edge
        size_t src{0};  

        for (char ch : pattern) {
            if (!t[src].count(ch)) {
                t[src].emplace(ch, ++node_cntr);
            }
            src = t[src][ch];
        }
    }

    return t;
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