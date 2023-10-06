#include <algorithm> // std::min
#include <iostream>
#include <iterator>
#include <queue> // std::priority_queue
#include <sstream>
#include <utility> // std::move, std::pair
#include <vector>

#include "iterator_range.h"
#include "parse.h"
#include "search_server.h"

SearchServer::SearchServer(std::istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
    size_t doc_id = 0;

    for (std::string current_document; std::getline(document_input, current_document);) {
        for (auto [word_view, count] : GetWordsCounterView(current_document)) {
            const std::string word{word_view};
            auto it = _unique_words.insert(word).first;
            _index[std::move(*it)].insert({doc_id, count});
        }
        ++doc_id;
    }
}

void SearchServer::AddQueriesStream(std::istream& query_input,
                                    std::ostream& search_results_output) {
    size_t max_num_of_relevant_docs = 5;

    for (std::string current_query; std::getline(query_input, current_query);) {
        std::unordered_map<size_t, size_t> docid_count;

        for (const std::string_view word : SplitIntoWordsView(current_query)) {
            for (auto [docid, count] : _index[word]) {
                docid_count[docid] += count;
            }
        }

        struct Compare {
            bool operator()(std::pair<size_t, size_t>& lhs, std::pair<size_t, size_t>& rhs) {
                return (lhs.second != rhs.second)
                           ? lhs.second < rhs.second
                           : lhs.first > rhs.first;
            }
        };

        std::priority_queue<std::pair<size_t, size_t>,
                            std::vector<std::pair<size_t, size_t>>,
                            Compare>
            max_heap;

        for (const auto& p : docid_count) {
            max_heap.push(p);
        }

        std::vector<std::pair<size_t, size_t>> search_results;
        size_t N = std::min(max_num_of_relevant_docs, max_heap.size());
        search_results.reserve(N);

        for (size_t i = 0; i < N; ++i) {
            search_results.push_back(max_heap.top());
            max_heap.pop();
        }

        search_results_output << current_query << ':';

        for (auto [docid, hitcount] : Head(search_results, 5)) {
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }

        search_results_output << std::endl;
    }
}
