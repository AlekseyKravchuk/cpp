#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string_view>

#include "iterator_range.h"

using SV_WordCounter = std::map<std::string_view, size_t>;

SearchServer::SearchServer(std::istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {

    size_t doc_id = 0;
    for (std::string current_document; std::getline(document_input, current_document);) {
        for (const auto& [word_sv, count] : GetWordsViewCounter(current_document)) {
            _index[std::string(word_sv)].insert({doc_id, count});
        }
        ++doc_id;
    }
}

void SearchServer::AddQueriesStream(
    std::istream& query_input, std::ostream& search_results_output) {
    for (std::string current_query; std::getline(query_input, current_query);) {
        const auto words = SplitIntoWords(current_query);

        std::map<size_t, size_t> docid_count;
        for (const auto& word : words) {
            for (const size_t docid : _inv_index_inst.Lookup(word)) {
                docid_count[docid]++;
            }
        }

        std::vector<std::pair<size_t, size_t>> search_results(docid_count.begin(), docid_count.end());

        std::sort(
            std::begin(search_results),
            std::end(search_results),
            [](std::pair<size_t, size_t> lhs, std::pair<size_t, size_t> rhs) {
                int64_t lhs_docid = lhs.first;
                auto lhs_hit_count = lhs.second;
                int64_t rhs_docid = rhs.first;
                auto rhs_hit_count = rhs.second;

                return std::make_pair(lhs_hit_count, -lhs_docid) > std::make_pair(rhs_hit_count, -rhs_docid);
            });

        search_results_output << current_query << ':';

        for (auto [docid, hitcount] : Head(search_results, 5)) {
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }

        search_results_output << std::endl;
    }
}

void InvertedIndex::Add(const std::string& document) {
    _docs.push_back(document);

    const size_t docid = _docs.size() - 1;
    for (const std::string_view word_sv : SplitIntoWords(document)) {
        _index[word_sv].push_back(docid);
    }
}

std::list<size_t> InvertedIndex::Lookup(const std::string& word) const {
    if (auto it = _index.find(word); it != _index.end()) {
        return it->second;
    } else {
        return {};
    }
}
