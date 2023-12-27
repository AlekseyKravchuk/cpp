#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "join_collection.h"
#include "test_runner.h"

using std::map;
using std::string;
using std::vector;

using edges = map<char, int>;
using trie = vector<edges>;

trie build_trie(vector<string>& patterns) {
    uint MAX_PATTERN_LENGTH = 10'001;
    trie pref_tree(MAX_PATTERN_LENGTH);
    uint node_cntr = 0;

    for (const auto& pattern : patterns) {
        uint src = 0;

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

std::ostream& operator<<(std::ostream& os, const trie& pref_tree) {
    for (size_t i = 0; i < pref_tree.size(); ++i) {
        for (const auto& [ch, dst_vertice] : pref_tree[i]) {
            os << i << "->" << dst_vertice << ":" << ch << "\n";
        }
    }

    return os;
}

void Test_1() {
    vector<string> patterns = {"ATA"};

    string expected = JoinAddExtraNewline('\n', vector<string>{
                                                    "0->1:A",
                                                    "1->2:T",
                                                    "2->3:A"});

    trie t = build_trie(patterns);

    std::ostringstream oss;
    oss << t;
    ASSERT_EQUAL(expected, oss.str());
}

void Test_2() {
    vector<string> patterns = {
        "AT",
        "AG",
        "AC"};

    std::set<string> expected_set = {
        "0->1:A",
        "1->2:T",
        "1->3:G",
        "1->4:C"};

    trie t = build_trie(patterns);

    std::ostringstream oss;
    oss << t;
    ASSERT_EQUAL(SplitBy(oss.str(), '\n'),
                 expected_set);
}

void Test_3() {
    vector<string> patterns = {
        "ATAGA",
        "ATC",
        "GAT"};

    std::set<string> expected_set = {
        "0->1:A",
        "1->2:T",
        "2->3:A",
        "3->4:G",
        "4->5:A",
        "2->6:C",
        "0->7:G",
        "7->8:A",
        "8->9:T"};

    trie prefix_tree = build_trie(patterns);

    std::ostringstream oss;
    oss << prefix_tree;
    ASSERT_EQUAL(SplitBy(oss.str(), '\n'),
                 expected_set);
}

void Test_4() {
    string fname = "case_04.txt";
    std::ifstream input(fname);
    if (!input) {
        std::cerr << "File\"" << fname << "\" is not opened. Exiting." << std::endl;
    }

    vector<string> patterns;
    uint n;
    input >> n >> std::ws;
    for (string line; std::getline(input, line) && !line.empty();) {
        patterns.push_back(line);
    }

    trie prefix_tree = build_trie(patterns);

    string fname_expected = "case_04_expected.txt";
    std::ifstream input_expected(fname_expected);
    if (!input_expected) {
        std::cerr << "File\"" << fname_expected << "\" is not opened. Exiting." << std::endl;
    }

    std::set<string> expected_set;
    for (string line; std::getline(input_expected, line);) {
        expected_set.insert(line);
    }

    std::ostringstream oss;
    oss << prefix_tree;

    ASSERT_EQUAL(SplitBy(oss.str(), '\n'),
                 expected_set);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test_1);
    RUN_TEST(tr, Test_2);
    RUN_TEST(tr, Test_3);
    RUN_TEST(tr, Test_4);

    return 0;
}