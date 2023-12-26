#include <fstream>
#include <iostream>
#include <limits>  // std::numeric_limits
#include <map>
#include <string>
#include <vector>

#define _GLIBCXX_DEBUG 1  // включить режим отладки

using std::map;
using std::string;
using std::vector;

using edges = map<char, int>;
using trie = vector<edges>;

// TODO:
trie build_trie(vector<string>& patterns) {
    uint MAX_PATTERN_LENGTH = 101;
    trie pref_tree(MAX_PATTERN_LENGTH);
    uint node_cntr = 0;

    for (const auto& pattern : patterns) {
        uint src = 0;
        uint dst = 0;

        for (auto ch : pattern) {
            if (pref_tree[src].count(ch)) {
                src = pref_tree[src].at(ch);
                continue;
            } else {
                dst = node_cntr + 1;
                pref_tree[src].emplace(ch, dst);
                src = dst;
                ++node_cntr;
            }
        }
    }

    return pref_tree;
}

int main() {
    size_t n;

#ifdef _GLIBCXX_DEBUG
    // string fname = "input_2.txt";
    string fname = "input_3.txt";
    std::ifstream input(fname);  // configuring input from the file
    if (!input) {
        std::cerr << "File \"" << fname << "\" is not opened. Exiting." << std::endl;
    }
    std::streambuf* cinbuf = std::cin.rdbuf();  // save old buf
    std::cin.rdbuf(input.rdbuf());              // redirect std::cin to "input" file stream
#endif                                          // _GLIBCXX_DEBUG

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

#ifdef _GLIBCXX_DEBUG
    std::cin.rdbuf(cinbuf);  // reset to standard input again
#endif

    return 0;
}