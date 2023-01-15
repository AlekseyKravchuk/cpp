#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main() {
    set<string> query_words({"white"s, "cat"s, "long"s, "tail"});

    map<string, set<int>> inverted_index{
        {"colorful"s, {0}},
        {"parrot"s, {0}},
        {"green"s, {0}},
        {"green", {0}},
        {"wings"s, {0}},
        {"red"s, {0, 2}},
        {"tail"s, {0, 2}},
        {"lost"s, {0}},
        {"grey"s, {1}},
        {"found"s, {1, 2}},
        {"white"s, {2}},
        {"cat"s, {2}},
        {"long"s, {2}}};

    // not initialized map yet
    map<int, int> id_relevance_mapping;

    for (const auto& query_word : query_words) {
        // inverted_index[query_word] дает множество ID документов, в которых содержится слово "query_word"
        for (const auto doc_id : inverted_index[query_word]) {
            id_relevance_mapping[doc_id]++;
        }
    }

    cout << "Program is executing ..." << endl;

    return 0;
}
