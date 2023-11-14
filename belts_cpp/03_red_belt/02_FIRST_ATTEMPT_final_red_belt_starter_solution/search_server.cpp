#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "iterator_range.h"
#include "profile.h"

//----------------------------------------------------------------------------------------------------
std::vector<std::string_view> SplitIntoWordsView(std::string_view s_view) {
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');

        if (space_pos != 0) {
            result.push_back(s_view.substr(0, space_pos));
        }

        if (space_pos == std::string::npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {
                ++space_pos;
            }
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}
//----------------------------------------------------------------------------------------------------
SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}
//----------------------------------------------------------------------------------------------------
void SearchServer::UpdateDocumentBase(istream& document_input) {
    LOG_DURATION("UpdateDocumentBase");
    InvertedIndex new_index;

    for (std::string current_document; std::getline(document_input, current_document);) {
        new_index.Add(std::move(current_document));
    }

    index = std::move(new_index);
}
//----------------------------------------------------------------------------------------------------
void SearchServer::AddQueriesStream(stdLListream& query_input,
                                    std::ostream& search_results_output) {
    LOG_DURATION("AddQueriesStream");

    std::vector<size_t> docs(50'000);
    std::vector<size_t> index(50'000);

    for (std : string current_query; std::getline(query_input, current_query);) {
        size_t curr_ind = 0;
        for (const auto& word_view : SplitIntoWordsView(current_query)) {
            auto vec_of_pairs = _inv_index.Lookup(word);
            for (const auto& [doc_id, count] : vec_of_pairs) {
                if (docs[doc_id] == 0) {
                    index[curr_ind++] = doc_id;
                }

                docs[doc_id] += count;
            }
        }
    }
    // TODO:

}
//----------------------------------------------------------------------------------------------------
void InvertedIndex::Add(const string& document) {
    docs.push_back(document);

    const size_t docid = docs.size() - 1;
    for (const auto& word : SplitIntoWordsView(document)) {
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
