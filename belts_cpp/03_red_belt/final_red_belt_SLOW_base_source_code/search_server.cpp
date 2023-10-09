#include "search_server.h"

#include "duration.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "iterator_range.h"

std::vector<std::string> SplitIntoWords(const std::string& line) {
    std::istringstream words_input(line);

    return {std::istream_iterator<std::string>(words_input),
            std::istream_iterator<std::string>()};
}

SearchServer::SearchServer(std::istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
    InvertedIndex new_inv_index;

    for (std::string current_document; std::getline(document_input, current_document);) {
        new_inv_index.Add(std::move(current_document));
    }

    _inv_index_inst = std::move(new_inv_index);
}

void SearchServer::AddQueriesStream(std::istream& query_input,
                                    std::ostream& search_results_output) {

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
    for (const auto& word : SplitIntoWords(document)) {
        _index[word].push_back(docid);
    }
}

std::list<size_t> InvertedIndex::Lookup(const std::string& word) const {
    if (auto it = _index.find(word); it != _index.end()) {
        return it->second;
    } else {
        return {};
    }
}
