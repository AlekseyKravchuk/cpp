#include "search_server.h"

#include "iterator_range.h"
#include "parse.h"

#include <algorithm>
#include <future>
#include <numeric>

InvertedIndex::InvertedIndex(std::istream& document_input) {
    for (std::string current_document; std::getline(document_input, current_document);) {
        _docs.push_back(std::move(current_document));
        size_t docid = _docs.size() - 1;

        for (std::string_view word : SplitIntoWordsView(_docs.back())) {
            auto& docids = _index[word];
            if (!docids.empty() && docids.back().docid == docid) {
                ++docids.back().hitcount;
            } else {
                docids.push_back({docid, 1});
            }
        }
    }
}

const std::vector<InvertedIndex::Entry>& InvertedIndex::Lookup(std::string_view word) const {
    static const std::vector<Entry> empty;
    if (auto it = _index.find(word); it != _index.end()) {
        return it->second;
    } else {
        return empty;
    }
}

void UpdateIndex(std::istream& document_input, Synchronized<InvertedIndex>& index) {
    InvertedIndex new_index(document_input);
    std::swap(index.GetAccess().ref_to_value, new_index);
}

void ProcessSearches(
    std::istream& query_input,
    std::ostream& search_results_output,
    Synchronized<InvertedIndex>& index_handle) {
    std::vector<size_t> docid_count;
    std::vector<int64_t> docids;

    for (std::string current_query; std::getline(query_input, current_query);) {
        const auto words = SplitIntoWordsView(current_query);

        {
            auto access = index_handle.GetAccess();

            // В отличие от однопоточной версии мы должны при каждом обращении
            // к индексу изменять размер векторов docid_count и docids, потому что
            // между последовательными итерациями цикла индекс может быть изменён
            // параллельным запуском функции UpdateIndex. Соответственно в новой
            // версии базы может быть другое количество документов.
            const size_t doc_count = access.ref_to_value.GetDocuments().size();
            docid_count.assign(doc_count, 0);
            docids.resize(doc_count);

            auto& index = access.ref_to_value;
            for (const auto& word : words) {
                for (const auto& [docid, hit_count] : index.Lookup(word)) {
                    docid_count[docid] += hit_count;
                }
            }
        }

        std::iota(docids.begin(), docids.end(), 0);
        {
            std::partial_sort(
                std::begin(docids),
                Head(docids, 5).end(),
                std::end(docids),
                [&docid_count](int64_t lhs, int64_t rhs) {
                    return std::pair(docid_count[lhs], -lhs) > std::pair(docid_count[rhs], -rhs);
                });
        }

        search_results_output << current_query << ':';
        for (size_t docid : Head(docids, 5)) {
            const size_t hit_count = docid_count[docid];
            if (hit_count == 0) {
                break;
            }

            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hit_count << '}';
        }
        search_results_output << '\n';
    }
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
    async_tasks.push_back(std::async(UpdateIndex, ref(document_input), ref(_inv_index)));
}

void SearchServer::AddQueriesStream(std::istream& query_input,
                                    std::ostream& search_results_output) {
    async_tasks.push_back(
        std::async(ProcessSearches,
                   ref(query_input),
                   ref(search_results_output),
                   ref(_inv_index)));
}