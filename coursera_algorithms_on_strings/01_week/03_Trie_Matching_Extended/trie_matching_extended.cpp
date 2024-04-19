#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator> // std::ostream_iterator
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord = false;
};

class Trie {
  private:
    TrieNode* root = nullptr;

  public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& s) {
        TrieNode* cur = root;

        for (const char& ch: s) {
            if (not cur->children.count(ch)) {
                cur->children[ch] = new TrieNode();
            }

            cur = cur->children[ch];
        }

        cur->isEndOfWord = true;
    }

    bool hasPrefix(const string& text, size_t start_position) {
        TrieNode* cur = root;

        for (size_t i = start_position; i < text.size(); i++) {
            const char ch = text[i];

            if (cur->children.count(ch)) {
                cur = cur->children[ch];
            } else {
                break;
            }

            if (cur->isEndOfWord) {
                return true;
            }
        }

        return false;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string text, pattern;
    cin >> text;

    uint64_t n;
    Trie* trie = new Trie();
    cin >> n;

    for (uint64_t i = 0; i < n; i++) {
        cin >> pattern;
        trie->insert(pattern);
    }

    vector<uint64_t> result;
    for (size_t i = 0; i < text.size(); i++) {
        if (trie->hasPrefix(text, i)) {
            result.push_back(i);
        }
    }

    copy(begin(result), end(result), ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}