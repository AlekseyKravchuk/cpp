#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "duration.h"
#include "iterator_range.h"

// // Использовалась эта реализации в "starter solution"
// vector<string> SplitIntoWords(const string& line) {
//     istringstream words_input(line);
//     return {istream_iterator<string>(words_input), istream_iterator<string>()};
// }

std::vector<std::string_view> SplitIntoWordsView(std::string_view view) {
    std::vector<std::string_view> result;
    size_t first_nonspace_pos = view.find_first_not_of(' ', 0);

    if (first_nonspace_pos == std::string_view::npos) {
        return result;
    }

    while (true) {
        size_t space_pos = view.find(' ', first_nonspace_pos);
        result.emplace_back(view.substr(first_nonspace_pos, space_pos - first_nonspace_pos));

        if (space_pos == view.npos) {
            break;
        } else {
            first_nonspace_pos = view.find_first_not_of(' ', space_pos);
        }

        if (first_nonspace_pos == std::string_view::npos) {
            break;
        }
    }

    return result;
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document);) {
        new_index.Add(move(current_document));
    }

    index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
    for (string current_query; getline(query_input, current_query);) {
        vector<string> words = SplitIntoWords(current_query);

        map<size_t, size_t> docid_count;
        for (const auto& word : words) {
            for (const size_t docid : index.Lookup(word)) {
                docid_count[docid]++;
            }
        }

        vector<pair<size_t, size_t>> search_results(
            docid_count.begin(), docid_count.end());
        sort(
            begin(search_results),
            end(search_results),
            [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                int64_t lhs_docid = lhs.first;
                auto lhs_hit_count = lhs.second;
                int64_t rhs_docid = rhs.first;
                auto rhs_hit_count = rhs.second;
                return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
            });

        search_results_output << current_query << ':';
        for (auto [docid, hitcount] : Head(search_results, 5)) {
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }
        search_results_output << endl;
    }
}

void InvertedIndex::Add(const string& document) {
    docs.push_back(document);

    const size_t docid = docs.size() - 1;
    for (const auto& word : SplitIntoWords(document)) {
        index[word].push_back(docid);
    }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return {};
    }
}
