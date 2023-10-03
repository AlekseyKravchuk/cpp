#include "search_server.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string_view>

#include "iterator_range.h"

// std::vector<std::string> SplitIntoWords(const std::string& line) {
//     std::istringstream words_input(line);

//     return {std::istream_iterator<std::string>(words_input),
//             std::istream_iterator<std::string>()};
// }

// remove leading and trailing whitespaces
void trim(std::string_view& s) {
    // remove leading whitespaces
    size_t pos = s.find_first_not_of(" ");
    if (pos != s.npos) {
        s.remove_prefix(pos);
    }

    // remove trailing whitespaces
    pos = s.find_last_not_of(" ");
    if (pos != s.npos) {
        s.remove_suffix(s.size() - (pos + 1));
    } else {
        s.remove_suffix(s.size());  // строка состоит только из пробельных символов
    }
}

std::vector<std::string_view> SplitIntoWords(std::string_view s_view) {
    trim(s_view);  // избавляемся от пробельных символов в начале и в конце строки
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');            // ищем позицию первого пробела в ещё не обработанной части
        result.push_back(s_view.substr(0, space_pos));  // помещаем подстроку-string_view в вектор

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') {  // обрабатываем ВСЕ пробелы между словами
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
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
