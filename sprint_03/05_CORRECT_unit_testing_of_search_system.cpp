#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
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
            _word2doc2freqs[word][docID] += inv_word_count;
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

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    // 10^(-6); constant used when comparing two document relevances (of type "double")
    const double _REASONABLE_ERROR = 1e-6;

    set<string> _stop_words;
    map<string, map<int, double>> _word2doc2freqs;
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

    template <typename Predicate>
    vector<Document> FindAllDocuments(const Query& query, Predicate pred) const {
        map<int, double> doc2relevance;

        for (const string& word : query.plus_words) {
            if (_word2doc2freqs.count(word) == 0) {
                continue;
            }

            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [docID, term_freq] : _word2doc2freqs.at(word)) {
                if (pred(docID, _documents.at(docID).status, _documents.at(docID).rating)) {
                    doc2relevance[docID] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word2doc2freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word2doc2freqs.at(word)) {
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
        assert(server.FindTopDocuments("in"s).size() == 0);
    }
}

void TestAddDocument() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
        server.AddDocument(33, "cat into door"s, DocumentStatus::ACTUAL, {2, 3, 4});
        const auto found_docs1 = server.FindTopDocuments("cat"s);
        assert(found_docs1.size() == 2);
        const Document& doc01 = found_docs1[0];
        assert(doc01.id == 33);
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

void TestMinusWords() {
    {
        SearchServer server;
        server.AddDocument(2, "красная синяя кошка за окном"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(34, "красная черная кошка за окном"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const auto found_docs = server.FindTopDocuments("красная кошка черная"s);
        assert(found_docs.size() == 2);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == 34);
        const auto found_docs1 = server.FindTopDocuments("красная кошка -черная"s);
        assert(found_docs1.size() == 1);
        const Document& doc1 = found_docs1[0];
        assert(doc1.id == 2);
    }
}

void TestMatchDocument() {
    SearchServer server;
    server.AddDocument(1, "красный белый синий"s, DocumentStatus::ACTUAL, {1, 2, 3});
    vector<string> found_doc;
    DocumentStatus status;
    tie(found_doc, status) = server.MatchDocument("белый синий"s, 1);
    assert(found_doc.size() == 2);
    vector<string> found_doc1;
    DocumentStatus status1;
    tie(found_doc1, status1) = server.MatchDocument("красный -синий"s, 1);
    assert(found_doc1.size() == 0);
    vector<string> found_doc2;
    DocumentStatus status2;
    tie(found_doc2, status2) = server.MatchDocument("коричневый кот"s, 1);
    assert(found_doc2.size() == 0);
}

void TestSortDocument() {
    SearchServer server;
    {
        server.AddDocument(2, "красный оранжевый белый"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(1, "красный синий кот"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(3, "красный оранжевый олигарх"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const auto found_docs = server.FindTopDocuments("красная кот оранжевый"s);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == 1);
    }
    {
        server.AddDocument(2, "красный синий белый кот"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(1, "красный синий синий кот"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const auto found_docs = server.FindTopDocuments("красная кот синий"s);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == 1);
    }
}

void TestRating() {
    SearchServer server;
    server.AddDocument(1, "red cat under roof"s, DocumentStatus::ACTUAL, {1, 2, 3});
    server.AddDocument(2, "black cat is running"s, DocumentStatus::ACTUAL, {1, 2, 3, 9});
    const auto found_docs = server.FindTopDocuments("cat"s);
    assert(found_docs[0].rating == 3);
    assert(found_docs[1].rating == 2);
}

void TestPredicate() {
    SearchServer server;
    server.AddDocument(22, "red cat under roof"s, DocumentStatus::ACTUAL, {1, 2, 3});
    server.AddDocument(23, "black cat is running"s, DocumentStatus::IRRELEVANT, {1, 2, 3});
    server.AddDocument(24, "black cat in runnings"s, DocumentStatus::BANNED, {1, 2, 3});
    server.AddDocument(25, "orange cat after runnings"s, DocumentStatus::REMOVED, {1, 2, 3});
    const auto found_docs = server.FindTopDocuments("cat"s, DocumentStatus::ACTUAL);
    assert(found_docs.size() == 1);
    const Document& doc0 = found_docs[0];
    assert(doc0.id == 22);
    const auto found_docs1 = server.FindTopDocuments("cat"s, DocumentStatus::IRRELEVANT);
    assert(found_docs1.size() == 1);
    const Document& doc01 = found_docs1[0];
    assert(doc01.id == 23);
    const auto found_docs2 = server.FindTopDocuments("cat"s, DocumentStatus::BANNED);
    assert(found_docs2.size() == 1);
    const Document& doc02 = found_docs2[0];
    assert(doc02.id == 24);
    const auto found_docs3 = server.FindTopDocuments("cat"s, DocumentStatus::REMOVED);
    assert(found_docs3.size() == 1);
    const Document& doc03 = found_docs3[0];
    assert(doc03.id == 25);
}

void TestCorrectRelevance() {
    SearchServer server;
    server.AddDocument(22, "house cristall and gold"s, DocumentStatus::ACTUAL, {1, 2, 3});
    server.AddDocument(23, "car green sky house"s, DocumentStatus::ACTUAL, {1, 2, 3});
    server.AddDocument(24, "yellow black white house"s, DocumentStatus::ACTUAL, {1, 2, 3});
    const auto found_docs = server.FindTopDocuments("cristall house gold"s);
    double ans1, ans2, ans3;
    ans1 = 2.0 * log(3.0) * (0.25) + log(1.0) * (0.25);
    ans2 = log(1.0) * (0.25);
    ans3 = log(3.0) * (0.25) + log(1.0) * (0.25);
    assert(found_docs[0].relevance == ans1);
    assert(found_docs[1].relevance == ans2);
    const auto found_docs1 = server.FindTopDocuments("house green"s);
    assert(found_docs1[0].relevance == ans3);
}
/*
Разместите код остальных тестов здесь
*/

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    TestAddDocument();
    TestMinusWords();
    TestSortDocument();
    TestMatchDocument();
    TestRating();
    TestPredicate();
    TestCorrectRelevance();
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}