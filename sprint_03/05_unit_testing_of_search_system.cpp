#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    int id;
    double relevance;
    int rating;

    bool operator==(const Document& doc) {
        if (doc.id == id && doc.relevance == relevance && doc.rating == rating) {
            return true;
        } else {
            return false;
        }
    }
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
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

class SearchServer {
   public:
    int GetDocumentCount() const {
        return _documents.size();
    }

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stop_words.insert(word);
        }
    }

    void AddDocument(int docID,
                     const string& document,
                     DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();

        for (const string& word : words) {
            _word2docID2freqs[word][docID] += inv_word_count;
        }
        _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});
    }

    // #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
    template <typename Predicate>
    vector<Document> FindTopDocuments(const string& raw_query, Predicate pred) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, pred);

        sort(matched_documents.begin(), matched_documents.end(),
             [this](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < this->_REASONABLE_ERROR) {
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

    // #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
    vector<Document> FindTopDocuments(const string& raw_query,
                                      DocumentStatus ext_status) const {
        return FindTopDocuments(raw_query, [ext_status](int document_id, DocumentStatus status, int rating) { return ext_status == status; });
    }

    // #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }
            if (_word2docID2freqs.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }
            if (_word2docID2freqs.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, _documents.at(document_id).status};
    }

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    // 10^(-6); constant used when comparing two document relevances (of type "double")
    const double _REASONABLE_ERROR = 1e-6;

    set<string> _stop_words;
    map<string, map<int, double>> _word2docID2freqs;
    map<int, DocumentData> _documents;

    bool IsStopWord(const string& word) const {
        return _stop_words.count(word) > 0;
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

    // static int ComputeAverageRating(const vector<int>& ratings) {
    //     return (ratings.empty()) ? 0 : reduce(ratings.begin(), ratings.end(), 0.0) / static_cast<double>(ratings.size());
    // }

    struct QueryWord {
        string data;
        bool is_minus;
        bool isStopWord;
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
            if (!query_word.isStopWord) {
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
        return log(GetDocumentCount() * 1.0 / _word2docID2freqs.at(word).size());
    }

    template <typename Predicate>
    vector<Document> FindAllDocuments(const Query& query, Predicate pred) const {
        map<int, double> doc2relevance;

        for (const string& word : query.plus_words) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }

            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [docID, term_freq] : _word2docID2freqs.at(word)) {
                if (pred(docID, _documents.at(docID).status, _documents.at(docID).rating)) {
                    doc2relevance[docID] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word2docID2freqs.at(word)) {
                doc2relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : doc2relevance) {
            matched_documents.push_back(
                {document_id, relevance, _documents.at(document_id).rating});
        }
        return matched_documents;
    }
};

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}

void TestSearchAddedDocumentByQueryContainingWordsFromDocument() {
    vector<string> contents = {
        /* 0 */"cats in the city"s,
        /* 1 */"some incident occured in the city regarding famous cats"s,
        /* 2 */"cats live in a house near the mountains, these cats are very pleasant"s,
        /* 3 */"my father teaches mathematics"s,
        /* 4 */"there are different breeds of dogs"s,
        /* 5 */"cats make their owners happy"s,
        /* 6 */"the tanks full of water have fish and turtles in them"s,
        /* 7 */"my family always eats breakfast and dinner together"s,
        /* 8 */"in my city, there is a post office where people mail letters"s,
        /* 9 */"I am happy to live in my city"s};

    const vector<vector<int>> ratings_vec = {
        {1, 2, 3, 2, 4},
        {4, 5, 3},
        {5, 5, 5},
        {4, 4, 5},
        {3, 5, 4},
        {3, 2, 3},
        {3, 4, 3},
        {4, 5, 3, 4},
        {3, 5, 5},
        {5, 5, 3}};

    const string wordsToSearchFor = "city happy"s;

    // добавляем документы в базу данных
    {
        SearchServer server;
        int numCounter = static_cast<int>(contents.size());
        
        for (int id = 0; id < numCounter; ++id) {
            server.AddDocument(id, contents[id], DocumentStatus::ACTUAL, ratings_vec[id]);
        }

        const auto found_docs = server.FindTopDocuments(wordsToSearchFor);
        cout << "Found documents returned."s << endl;
        // assert(found_docs.size() == 1);
        // const Document& doc0 = found_docs[0];
        // assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        // SearchServer server;
        // server.SetStopWords("in the"s);
        // server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        // assert(server.FindTopDocuments("in"s).empty());
    }
}

/*
Разместите код остальных тестов здесь
*/

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    // TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно

    TestSearchAddedDocumentByQueryContainingWordsFromDocument();

    cout << "Search server testing finished"s << endl;
}