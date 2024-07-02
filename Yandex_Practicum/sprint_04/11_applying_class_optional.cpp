#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
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

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
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
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : _stopWords(MakeUniqueNonEmptyStrings(stop_words)) {}

    // Invoke delegating constructor from string container: SearchServer(SplitIntoWords(stop_words_text))
    explicit SearchServer(const string& stop_words_text)
        : SearchServer(SplitIntoWords(stop_words_text)) {}

    int GetDocumentCount() const { return _documents.size(); }

    // Document will be added in all cases except:
    // 1) docID < 0;
    // 2) document with given docID already exists in "_documents" data base
    // 3) if at least one word in the document contains special character
    [[nodiscard]] bool AddDocument(int docID,
                                   const string& raw_document,
                                   DocumentStatus status,
                                   const vector<int>& ratings) {
        bool documentAlreadyExists = _documents.find(docID) != _documents.end();
        if (docID < 0 || documentAlreadyExists) {
            return false;
        }

        vector<string> wordsNoStop;
        bool checkNoSpecialChars = SplitIntoWordsNoStop(raw_document, wordsNoStop);
        if (!checkNoSpecialChars) {
            return false;
        }

        const double inv_word_count = 1.0 / wordsNoStop.size();
        for (const string& word : wordsNoStop) {
            _word_docID_freqs[word][docID] += inv_word_count;
        }
        _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});

        _numberingInOrder.push_back(docID);

        return true;
    }

    // TO DO: изменить сигнатуру метода!!!
    // #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
    template <typename DocumentPredicate>
    optional<vector<Document>> FindTopDocuments(const string& rawQuery,
                                                DocumentPredicate document_predicate) const {
        Query parsedQuery;
        vector<Document> matched_documents;

        bool parseQueryResult = ParseQuery(rawQuery, parsedQuery);
        if (!parseQueryResult || parsedQuery.plus_words.empty()) {
            return nullopt;
        }

        matched_documents = FindAllDocuments(parsedQuery, document_predicate);
        if (!matched_documents.empty()) {
            sort(matched_documents.begin(), matched_documents.end(),
                 [](const Document& lhs, const Document& rhs) {
                     double reasonable_error = 1e-6;
                     if (abs(lhs.relevance - rhs.relevance) < reasonable_error) {
                         return lhs.rating > rhs.rating;
                     } else {
                         return lhs.relevance > rhs.relevance;
                     }
                 });
        }

        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }

        return matched_documents;
    }

    // #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
    optional<vector<Document>> FindTopDocuments(const string& rawQuery,
                                                DocumentStatus status) const {
        return FindTopDocuments(rawQuery,
                                [status](int document_id, DocumentStatus document_status, int rating) {
                                    return document_status == status;
                                });
    }

    // #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
    optional<vector<Document>> FindTopDocuments(const string& rawQuery) const {
        return FindTopDocuments(rawQuery, DocumentStatus::ACTUAL);
    }


    // First component of tuple is a vector of PLUS-query-words found in document specified by docID
    // Second component of a tuple is STATUS of document specified by docID
    optional<tuple<vector<string>, DocumentStatus>> MatchDocument(const string& rawQuery, int docID) const {
        // tuple<vector<string>, DocumentStatus> result;

        Query parsedQuery;  // parsedQuery is represented by 2 sets of string: 1)plus_words and 2)minus_words

        // placeholder for first component (vector<string>) of a "result" tuple
        vector<string> intersectionWithPlusWords;

        // / placeholder for second component (DocumentStatus) of a "result" tuple
        DocumentStatus status;

        // return false if document with "docID" isn't found
        if (_documents.count(docID)) {
            status = _documents.at(docID).status;
        } else {
            return nullopt;
        }

        // return false if:
        // 1) "rawQuery" contains special characters or redundant minuses
        // 2) "plus_words" set of query is empty
        bool parseQueryResult = ParseQuery(rawQuery, parsedQuery);

        if (!parseQueryResult || parsedQuery.plus_words.empty()) {
            return nullopt;
        }

        // return true although some minus-word presents in document specified by docID,
        // but with empty "intersectionWithPlusWords"
        for (const string& minusWord : parsedQuery.minus_words) {
            // if "minusWord" is found in inverted index
            if (_word_docID_freqs.count(minusWord)) {
                // if "docID" key is found in map "docID : frequency"
                if (_word_docID_freqs.at(minusWord).count(docID)) {
                    // return {intersectionWithPlusWords, status};
                    return std::make_tuple(intersectionWithPlusWords, status);
                }
            }
        }

        for (const string& plusWord : parsedQuery.plus_words) {
            if (_word_docID_freqs.count(plusWord)) {
                if (_word_docID_freqs.at(plusWord).count(docID)) {
                    if (!IsStopWord(plusWord)) {
                        intersectionWithPlusWords.push_back(plusWord);
                    }
                }
            }
        }

        return std::make_tuple(intersectionWithPlusWords, status);
    }

    // return document identifier by its serial number
    int GetDocumentId(int serial_number) const {
        if (serial_number < 0 || serial_number > _documents.size()) {
            return SearchServer::INVALID_DOCUMENT_ID;
        } else {
            return _numberingInOrder[serial_number];
        }
    }

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    struct QueryWord {
        string data;
        bool is_minus;
        bool isStopWord;
    };

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    set<string> _stopWords;
    map<string, map<int, double>> _word_docID_freqs;
    map<int, DocumentData> _documents;
    vector<int> _numberingInOrder;

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stopWords.insert(word);
        }
    }

    static bool IsWordWithoutSpecialChars(const string word2Check) {
        // возвращает true, если НИ ОДИН из символов проверяемого слова не принадлежит диапазону [\0; "SPACE")
        return none_of(word2Check.begin(),
                       word2Check.end(),
                       [](char ch) { return ch >= '\0' && ch < ' '; });
    }

    static bool IsValidQueryWord(const string& queryWord) {
        size_t minusCounter = 0;

        // проверка на несколько "-" в начале слова и на слово, состоящее из одного "-"
        // минусы в середине слова допускаются, например: "иван-чай", "-иван-чай"
        for (auto& ch : queryWord) {
            if (ch == '-') {
                ++minusCounter;
                if ((minusCounter > 1) || (queryWord.size() == 1)) {
                    return false;
                }
            } else {
                break;
            }
        }

        // проверка на "-" в конце слова
        if (queryWord.back() == '-') {
            return false;
        }

        // конечная проверка на отстутствие спец. символов
        return IsWordWithoutSpecialChars(queryWord);
    }

    static bool isInvalidQueryWord(const string& queryWord) {
        return !IsValidQueryWord(queryWord);
    }

    bool IsStopWord(const string& word) const {
        return _stopWords.count(word) > 0;
    }

    // SplitIntoWordsNoStop(...) returns "false" if at least one word within the document content contains special characters
    // otherwise return "true" and place vector of words composing the document content (excluding stop-words) vector "words"
    [[nodiscard]] bool SplitIntoWordsNoStop(const string& text, vector<string>& words) const {
        for (const string& word : SplitIntoWords(text)) {
            if (IsWordWithoutSpecialChars(word)) {
                if (!IsStopWord(word)) {
                    words.push_back(word);
                }
            } else {  // if at least one word contains special characters
                return false;
            }
        }
        return true;
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

    QueryWord ParseQueryWord(string queryWord) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (queryWord[0] == '-') {
            is_minus = true;
            queryWord = queryWord.substr(1);
        }
        return {queryWord, is_minus, IsStopWord(queryWord)};
    }

    [[nodiscard]] bool ParseQuery(const string& rawQuery, Query& parsedQuery) const {
        for (const string& word : SplitIntoWords(rawQuery)) {
            if (isInvalidQueryWord(word)) {
                return false;
            }

            const QueryWord queryWord = ParseQueryWord(word);
            if (queryWord.is_minus) {
                parsedQuery.minus_words.insert(queryWord.data);
            } else {
                parsedQuery.plus_words.insert(queryWord.data);
            }
        }
        return true;
    }

    // Existence required
    double ComputeInvertedDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / _word_docID_freqs.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeInvertedDocumentFreq(word);
            for (const auto [document_id, term_freq] : _word_docID_freqs.at(word)) {
                const auto& document_data = _documents.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word_docID_freqs.at(word)) {
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
    (void) search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }
    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }
    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }
    if (const auto documents = search_server.FindTopDocuments("--пушистый"s)) {
        for (const Document& document : *documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
} 