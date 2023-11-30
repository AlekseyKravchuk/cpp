#include "search_server.h"
#include "iterator_range.h"
#include "profile.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

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

std::unordered_map<std::string_view, size_t> SplitIntoWordsViewCounted(std::string_view view) {
    std::unordered_map<std::string_view, size_t> result;
    size_t first_nonspace_pos = view.find_first_not_of(' ', 0);

    if (first_nonspace_pos == std::string_view::npos) {
        return result;
    }

    while (true) {
        size_t space_pos = view.find(' ', first_nonspace_pos);
        ++result[view.substr(first_nonspace_pos, space_pos - first_nonspace_pos)];

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

SearchServer::SearchServer(std::istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
    InvertedIndex new_index;

    for (std::string current_document; std::getline(document_input, current_document);) {
        new_index.Add(std::move(current_document));
    }

    _inv_index = std::move(new_index);
}

void SearchServer::AddQueriesStream(std::istream& query_input,
                                    std::ostream& search_results_output) {
    size_t MAX_DOCUMENT_INPUT = 50'000;
    std::vector<size_t> stats(MAX_DOCUMENT_INPUT);
    std::vector<size_t> handling_order(MAX_DOCUMENT_INPUT);

    for (std::string current_query; std::getline(query_input, current_query);) {
        size_t curr_index = 0;

        for (auto& word : SplitIntoWordsView(current_query)) {
            for (auto [docid, count] : _inv_index.Lookup(word)) {
                if (stats[docid] == 0) {
                    handling_order[curr_index++] = docid;
                }
                stats[docid] += count;
            }
        }

        std::vector<std::pair<size_t, size_t>> search_results;
        search_results.reserve(MAX_DOCUMENT_INPUT);

        for (size_t i = 0; i < curr_index; ++i) {
            size_t docid{0}, count{0};
            std::swap(count, stats[handling_order[i]]);
            std::swap(docid, handling_order[i]);
            search_results.emplace_back(docid, count);
        }

        const size_t MAX_ANSWERS_COUNT = 5;
        size_t answers_count = std::min(search_results.size(), MAX_ANSWERS_COUNT);

        std::partial_sort(search_results.begin(),
                          search_results.begin() + answers_count,
                          search_results.end(),
                          [](std::pair<size_t, size_t> lhs, std::pair<size_t, size_t> rhs) {
                              return lhs.second != rhs.second
                                         ? lhs.second > rhs.second
                                         : lhs.first < rhs.first;
                          });

        search_results_output << current_query << ':';

        for (const auto& [docid, hitcount] : Head(search_results, static_cast<int>(answers_count))) {
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }
        search_results_output << endl;
    }
}

void InvertedIndex::Add(string&& document) {
    size_t docid = _doccount++;

    for (auto [word, count] : SplitIntoWordsViewCounted(std::move(document))) {
        _index[std::string(word)].emplace_back(docid, count);
    }
}

std::vector<std::pair<size_t, size_t>> InvertedIndex::Lookup(std::string_view word) const {
    std::string key{word};
    
    return _index.count(key)
               ? _index.at(std::move(key))
               : std::vector<std::pair<size_t, size_t>>{};
}
