#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>  // for std::istringstream()
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id), relevance(relevance), rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

// converts space-separated string "text" into vector of strings
vector<string> SplitIntoWords(const string& text) {
    istringstream iss(text);
    vector<string> words;
    string word;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
   public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words) : _stopWords(MakeUniqueNonEmptyStrings(stop_words)) {}

    // Invoke delegating constructor from string container
    explicit SearchServer(const string& stop_words_text) : SearchServer(SplitIntoWords(stop_words_text)) {}

    int GetDocumentCount() const { return _documents.size(); }

    // TO DO: реализовать новый интерфейс
    [[nodiscard]] bool AddDocument(int docID,
                                   const string& document,
                                   DocumentStatus status,
                                   const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            _word2doc2freqs[word][docID] += inv_word_count;
        }
        _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});
    }

    // TO DO: реализовать новый интерфейс
    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& raw_query,
                                        DocumentPredicate document_predicate,
                                        vector<Document>& result) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    // TO DO: реализовать новый интерфейс
    [[nodiscard]] bool FindTopDocuments(const string& raw_query,
                                        DocumentStatus status,
                                        vector<Document>& result) const {
        return FindTopDocuments(raw_query,
                                [status](int document_id, DocumentStatus document_status, int rating) {
                                    return document_status == status;
                                });
    }

    // TO DO: реализовать новый интерфейс
    [[nodiscard]] bool FindTopDocuments(const string& raw_query,
                                        vector<Document>& result) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    // TO DO: реализовать новый интерфейс
    [[nodiscard]] bool MatchDocument(const string& raw_query,
                                     int document_id,
                                     tuple<vector<string>, DocumentStatus>& result) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (_word2doc2freqs.count(word) == 0) {
                continue;
            }
            if (_word2doc2freqs.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (_word2doc2freqs.count(word) == 0) {
                continue;
            }
            if (_word2doc2freqs.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, _documents.at(document_id).status};
    }

    int GetDocumentId(int index) const {
        // ...
    }

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    const set<string> _stopWords;
    map<string, map<int, double>> _word2doc2freqs;
    map<int, DocumentData> _documents;

    bool IsStopWord(const string& word) const {
        return _stopWords.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / _word2doc2freqs.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (_word2doc2freqs.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : _word2doc2freqs.at(word)) {
                const auto& document_data = _documents.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word2doc2freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word2doc2freqs.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                {document_id, relevance, _documents.at(document_id).rating});
        }
        return matched_documents;
    }
};

// ==================== для примера =========================

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    SearchServer search_server("и в на"s);
    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void)search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }
    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }
    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }
    vector<Document> documents;
    if (search_server.FindTopDocuments("--пушистый"s, documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
}