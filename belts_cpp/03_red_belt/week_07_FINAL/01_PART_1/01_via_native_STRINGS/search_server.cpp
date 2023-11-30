#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "iterator_range.h"

// =================================================================================================
std::vector<std::string> SplitIntoWords(std::string&& str) {
    std::vector<std::string> result;
    size_t nonspace_pos{0}, pos{0};

    while (true) {
        nonspace_pos = str.find_first_not_of(' ', pos);
        if (nonspace_pos == std::string::npos) {
            break;
        }

        pos = str.find_first_of(' ', nonspace_pos);
        result.push_back(
            pos == std::string::npos
                ? str.substr(nonspace_pos)
                : str.substr(nonspace_pos, pos - nonspace_pos));

        nonspace_pos = pos;
    }

    return result;
}

// =================================================================================================
std::map<std::string, size_t> SplitIntoWordsCounted(std::string&& str) {
    std::map<std::string, size_t> result;
    size_t nonspace_pos{0}, pos{0};

    while (true) {
        nonspace_pos = str.find_first_not_of(' ', pos);
        if (nonspace_pos == std::string::npos) {
            break;
        }

        pos = str.find_first_of(' ', nonspace_pos);
        size_t count = (pos == std::string::npos)
                           ? std::string::npos
                           : pos - nonspace_pos;

        ++result[str.substr(nonspace_pos, count)];

        nonspace_pos = pos;
    }

    return result;
}

// =================================================================================================

SearchServer::SearchServer(std::istream& document_input) {
    UpdateDocumentBase(document_input);
}
// =================================================================================================

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document);) {
        new_index.Add(std::move(current_document));
    }

    _inv_index = std::move(new_index);
}
// =================================================================================================

void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) {
    size_t DOC_INPUT_MAX_SIZE = 50'000;
    std::vector<size_t> stats(DOC_INPUT_MAX_SIZE);
    std::vector<size_t> handling_order(DOC_INPUT_MAX_SIZE);

    for (string current_query; std::getline(query_input, current_query);) {
        size_t curr_index = 0;

        for (auto& word : SplitIntoWords(std::move(current_query))) {
            for (const auto& [docid, count] : _inv_index.Lookup(std::move(word))) {
                if (stats[docid] == 0) {
                    handling_order[curr_index++] = docid;
                }
                stats[docid] += count;
            }
        }

        std::vector<std::pair<size_t, size_t>> search_results;
        search_results.reserve(DOC_INPUT_MAX_SIZE);

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
                          [](std::pair<size_t, size_t>& lhs, std::pair<size_t, size_t>& rhs) {
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
// =================================================================================================

void InvertedIndex::Add(string&& document) {
    const size_t docid = _doc_count++;

    for (auto& [word, count] : SplitIntoWordsCounted(std::move(document))) {
        _index[std::move(word)].emplace_back(docid, count);
    }
}
// =================================================================================================

std::vector<std::pair<size_t, size_t>> InvertedIndex::Lookup(std::string&& word) const {
    return _index.count(word)
               ? _index.at(word)
               : std::vector<std::pair<size_t, size_t>>{};
}
// =================================================================================================
