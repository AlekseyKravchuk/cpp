#include <algorithm> // std::max
#include <cstddef>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility> // std::pair, std::move
#include <vector>

#include "../../test_runner.h"

template <typename It>
class IteratorRange {
  public:
    IteratorRange(It first, It last) : first(first), last(last) {}

    It begin() const {
        return first;
    }

    It end() const {
        return last;
    }

    size_t size() const {
        return last - first;
    }

  private:
    It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
    return IteratorRange(std::begin(c), std::begin(c) + std::min<size_t>(std::max(top, 0), c.size()));
}

template <typename Container>
std::string Join(char c, const Container& cont) {
    std::ostringstream os;

    for (const auto& item : Head(cont, cont.size() - 1)) {
        os << item << c;
    }

    os << *rbegin(cont);
    return os.str();
}

// remove leading and trailing whitespaces
void trim(std::string_view& s_view) {
    // remove leading whitespaces
    size_t pos = s_view.find_first_not_of(" ");
    if (pos != s_view.npos) {
        s_view.remove_prefix(pos);
    }

    // remove trailing whitespaces
    pos = s_view.find_last_not_of(" ");
    if (pos != s_view.npos) {
        s_view.remove_suffix(s_view.size() - (pos + 1));
    } else {
        s_view.remove_suffix(s_view.size()); // строка состоит только из пробельных символов
    }
}

std::unordered_map<std::string_view, size_t> GetWordsCounterView(std::string_view s_view) {
    trim(s_view); // избавляемся от пробельных символов в начале и в конце string_view
    std::unordered_map<std::string_view, size_t> result;

    while (true) {
        size_t space_pos = s_view.find(' '); // ищем позицию первого пробела в ещё не обработанной части
        ++result[s_view.substr(0, space_pos)];

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') { // обрабатываем ВСЕ пробелы между словами
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

std::vector<std::string_view> SplitIntoWordsView(std::string_view s_view) {
    trim(s_view); // избавляемся от пробельных символов в начале и в конце строки
    std::vector<std::string_view> result;

    while (true) {
        size_t space_pos = s_view.find(' ');           // ищем позицию первого пробела в ещё не обработанной части
        result.push_back(s_view.substr(0, space_pos)); // помещаем подстроку-string_view в вектор

        if (space_pos == s_view.npos) {
            break;
        } else {
            while (s_view[space_pos] == ' ') { // обрабатываем ВСЕ пробелы между словами
                ++space_pos;
            }

            // откусываем от "string_view" уже обработанный кусок: в качестве параметра указываем длину префикса, который нужно откусить
            s_view.remove_prefix(space_pos);
        }
    }

    return result;
}

class SearchServer {
  public:
    SearchServer() = default;

    explicit SearchServer(std::istream& document_input) {
        UpdateDocumentBase(document_input);
    }

    void UpdateDocumentBase(std::istream& document_input) {
        size_t doc_id = 0;
        for (std::string current_document; std::getline(document_input, current_document);) {
            for (auto [word_view, count] : GetWordsCounterView(current_document)) {
                const std::string word{word_view};
                // auto [it, success] = _unique_words.insert(word);
                auto it = _unique_words.insert(word).first;

                // if (!success) {
                //     it = _unique_words.find(word);
                // }
                _index[std::move(*it)].insert({doc_id, count});
            }
            ++doc_id;
        }
    }

    void AddQueriesStream(std::istream& query_input,
                          std::ostream& search_results_output,
                          size_t max_num_of_relevant_docs = 5) {
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
                                Compare> max_heap;

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

  private:
    // word : { {doc_id_1, number_of_occurrences_1},
    //          {doc_id_2, number_of_occurrences_2},
    //          {doc_id_6, number_of_occurrences_6} }
    std::unordered_map<std::string_view, std::map<size_t, size_t>> _index;
    std::unordered_set<std::string> _unique_words;
};

std::vector<std::string_view> SplitBy(std::string_view s, char sep) {
    std::vector<std::string_view> result;

    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(s.substr(0, pos));
        s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
    }

    return result;
}

std::string_view Strip(std::string_view s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }

    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }

    return s;
}

int main() {
    const std::vector<std::string> docs = {
        "london is the capital of great britain and london is my favourite city, so i think the UK is proud of it's capital",
        "i am travelling down the river",
        "i am dreaming of the london"};

    const std::vector<std::string> queries = {"london", "the", "is", "am"};

    const std::vector<std::string> expected = {
        Join(' ', std::vector{
                      "london:",
                      "{docid: 0, hitcount: 2}",
                      "{docid: 2, hitcount: 1}"}),
        Join(' ', std::vector{"the:", "{docid: 0, hitcount: 2}", "{docid: 1, hitcount: 1}", "{docid: 2, hitcount: 1}"}),
        Join(' ', std::vector{"is:", "{docid: 0, hitcount: 3}"}),
        Join(' ', std::vector{"am:", "{docid: 1, hitcount: 1}", "{docid: 2, hitcount: 1}"}),
    };

    std::istringstream docs_input(Join('\n', docs));
    std::istringstream queries_input(Join('\n', queries));
    // std::istringstream queries_input(Join(' ', queries)); // for debugging

    SearchServer srv;
    srv.UpdateDocumentBase(docs_input);

    std::ostringstream queries_output;
    srv.AddQueriesStream(queries_input, queries_output);

    const std::string result = queries_output.str();
    const auto lines = SplitBy(Strip(result), '\n');

    ASSERT_EQUAL(lines.size(), expected.size());

    for (size_t i = 0; i < lines.size(); ++i) {
        ASSERT_EQUAL(lines[i], expected[i]);
    }

    return 0;
}
