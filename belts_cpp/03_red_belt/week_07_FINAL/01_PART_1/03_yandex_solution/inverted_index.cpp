#include "inverted_index.h"

void InvertedIndex::Add(size_t doc_id, const std::string& document) {
    for (auto& [word, count]: SplitIntoWords(document)) {
        InsertCount(word, doc_id, count);
    }
}

void InvertedIndex::InsertCount(std::string_view word, size_t doc_id, size_t count) {
    auto it = helper_storage.find(word);
    if (it == helper_storage.end()) {
        data.emplace_back(word);
        it = helper_storage.emplace(data.back(), HelperCounter{}).first;
    }
    it->second[doc_id] += count;
}


void InvertedIndex::Insert(const InvertedIndex& other) {
    for (auto& [word, doc_to_cnt] : other.helper_storage) {
        for (auto& [doc_id, cnt] : doc_to_cnt) {
            InsertCount(word, doc_id, cnt);
        }
    }
}

const InvertedIndex::MainCounter & InvertedIndex::Lookup(string_view word) const {
    if (auto it = main_storage.find(word); it != main_storage.end()) {
        return it->second;
    }
    return EMPTY;
}

void InvertedIndex::SwapHelperAndMainStorage() {
    for (auto& [word, cnt] : helper_storage) {
        main_storage[word] = MainCounter(cnt.begin(), cnt.end());
    }
}
