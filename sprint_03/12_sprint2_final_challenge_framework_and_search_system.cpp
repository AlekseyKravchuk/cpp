#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// #include "search_server.h"

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
            _word2doc_freqs[word][docID] += inv_word_count;
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
            if (_word2doc_freqs.count(word) == 0) {
                continue;
            }
            if (_word2doc_freqs.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (_word2doc_freqs.count(word) == 0) {
                continue;
            }
            if (_word2doc_freqs.at(word).count(document_id)) {
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
    map<string, map<int, double>> _word2doc_freqs;
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

    /*     static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    } */

    static int ComputeAverageRating(const vector<int>& ratings) {
        return (ratings.empty()) ? 0 : reduce(ratings.begin(), ratings.end(), 0.0) / static_cast<double>(ratings.size());
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
        return log(GetDocumentCount() * 1.0 / _word2doc_freqs.at(word).size());
    }

    template <typename Predicate>
    vector<Document> FindAllDocuments(const Query& query, Predicate pred) const {
        map<int, double> doc2relevance;

        for (const string& word : query.plus_words) {
            if (_word2doc_freqs.count(word) == 0) {
                continue;
            }

            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [docID, term_freq] : _word2doc_freqs.at(word)) {
                if (pred(docID, _documents.at(docID).status, _documents.at(docID).rating)) {
                    doc2relevance[docID] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (_word2doc_freqs.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : _word2doc_freqs.at(word)) {
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

/*
   Подставьте сюда вашу реализацию макросов
   ASSERT, ASSERT_EQUAL, ASSERT_EQUAL_HINT, ASSERT_HINT и RUN_TEST
*/
#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define RUN_TEST(func)                  \
    {                                   \
        RunTestImpl(func);              \
        cerr << #func << " OK" << endl; \
    }

template <typename FuntionType>
void RunTestImpl(FuntionType func) {
    func();
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str,
                     const string& file, const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

void AssertImpl(bool expr, const string& exprAsStr,
                const string& fileName, const string& functionName,
                const int lineNumber, const string& hint) {
    if (!expr) {
        cout << fileName << "("s << lineNumber << "): "s
             << functionName << ": "s
             << "ASSERT("s << exprAsStr << ") failed."s;

        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

// -------- Начало модульных тестов поисковой системы ----------

// ^^^^^^^^^^^^^^^^^^^^ COMMON GLOBAL DATA AND FUNCTIONS FOR ALL TESTS ^^^^^^^^^^^^^^^^^^^^
const vector<string> contentsDB = {
    /* 00 */ "happy dog walking joyfully"s,
    /* 01 */ "dog likes walk and run"s,
    /* 02 */ "dog plays"s,
    /* 03 */ "dog sits on bed",
    /* 04 */ "dog was walking with his master at the street",
    /* 05 */ "lovely dog with big curly tail",
    /* 06 */ "      "s
    /* 07 */ "earth worm"s
    /* ,"extra document to fail test"s */};

const std::vector<int> test_ratings = {3, 0, 0, 0};
constexpr int test_document_id = 1;

// supplementaty funciton to compose documents DB from contentsDB
void ComposeDocumentsFromContentsDB(SearchServer& server) {
    for (size_t docID = 0; docID < contentsDB.size(); ++docID) {
        server.AddDocument(docID, contentsDB[docID], DocumentStatus::ACTUAL, test_ratings);
    }
}
// ============================================================================================

// ^^^^^^^^^^^^^^^^^^^^ #1 TestAddingDocumentsToTheServer ^^^^^^^^^^^^^^^^^^^^
// Проверка того, что добавленный документ находится системой по поисковому запросу, который содержит слова из документа.
void TestAddingDocumentsToTheServer() {
    SearchServer server;
    ASSERT(server.GetDocumentCount() == 0);

    ComposeDocumentsFromContentsDB(server);

    ASSERT(server.GetDocumentCount() == 7);

    for (const auto& word : SplitIntoWords(contentsDB[0])) {
        const auto found_documents = server.FindTopDocuments(word);
        ASSERT(found_documents.size() > 0);
    }
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #2 TestExcludeStopWordsFromAddedDocumentContent ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Проверка того, что поисковая система исключает стоп-слова из текста документов при их добавлении
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
                    "Stop words must be excluded from documents"s);
    }
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #3 TestMinusWords ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Поддержка минус-слов. Документы, содержащие минус-слова из поискового запроса, не должны включаться в результаты поиска.
void TestMinusWords() {
    const std::string queryWithMinusWords = "dog -walking"s;
    SearchServer server;
    ComposeDocumentsFromContentsDB(server);

    auto found_documents = server.FindTopDocuments(queryWithMinusWords);
    ASSERT_EQUAL_HINT(found_documents.size(), 4, "Server returns ONLY the documents without minus words, which were in query"s);

    found_documents = server.FindTopDocuments("dog walking -walking");
    ASSERT_EQUAL(found_documents.size(), 4);
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #4 TestMatching ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Соответствие документов поисковому запросу. При этом должны быть возвращены все слова из поискового запроса, присутствующие в документе.
// Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
void TestMatching() {
    SearchServer server;
    ComposeDocumentsFromContentsDB(server);

    for (size_t docID = 0; docID < contentsDB.size(); ++docID) {
        // проверка будет ли искать пустую строку
        auto [matching_words, _] = server.MatchDocument(""s, docID);
        ASSERT_HINT(matching_words.empty(), "matching empty word for docID"s);

        // проверка будет ли искать слово не из документа
        std::tie(matching_words, _) = server.MatchDocument("some extra words"s, docID);
        ASSERT_HINT(matching_words.empty(), "matching na words for docid"s);

        // находит все слова
        int docID2test = 4;
        if (docID == docID2test) {
            std::tie(matching_words, _) = server.MatchDocument("dog street master"s, docID2test);
            ASSERT_EQUAL_HINT(matching_words.size(), 3, "matching existing words for docid"s);

            std::tie(matching_words, _) = server.MatchDocument("dog -walking"s, docID2test);
            ASSERT_EQUAL_HINT(matching_words.size(), 0, "no matching minus words for docid"s);
        }
    }
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #5 TestSortByRelevance ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Сортировка найденных документов по релевантности.
// Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
void TestSortByRelevance() {
    SearchServer server;
    vector<string> v = SplitIntoWords("dog was happy to walk in park"s);

    server.AddDocument(0, "dog"s, DocumentStatus::ACTUAL, test_ratings);
    server.AddDocument(1, "dog was"s, DocumentStatus::ACTUAL, test_ratings);
    server.AddDocument(2, "dog was happy"s, DocumentStatus::ACTUAL, test_ratings);
    server.AddDocument(3, "dog was happy to"s, DocumentStatus::ACTUAL, test_ratings);
    server.AddDocument(4, "dog was happy to walk"s, DocumentStatus::ACTUAL, test_ratings);
    server.AddDocument(5, "dog was happy to walk in"s, DocumentStatus::ACTUAL, test_ratings);
    server.AddDocument(6, "dog was happy to walk in park"s, DocumentStatus::ACTUAL, test_ratings);

    const auto foundDocuments = server.FindTopDocuments("happy dog park walk"s);
    const std::vector<int> expectedDocumentIds = {6, 4, 5, 2, 0};

    ASSERT_EQUAL(server.GetDocumentCount(), 7);

    ASSERT_EQUAL(foundDocuments.size(), expectedDocumentIds.size());

    bool is_sorted_by_relevance =
        std::is_sorted(foundDocuments.begin(), foundDocuments.end(),
                       [](const Document& left, const Document& right) { return left.relevance > right.relevance; });

    ASSERT_HINT(is_sorted_by_relevance, "sorted by relevance"s);
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #6 TestRatingCalculation ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
void TestRatingCalculation() {
    SearchServer server;

    vector<int> test_ratings0 = {3, 0, 0, 0};
    vector<int> test_ratings1 = {1, 2, 4};
    vector<int> test_ratings2 = {1, 2, 5};
    vector<int> test_ratings3 = {-1, -2, -4};
    vector<int> test_ratings4 = {-1, -2, -5};
    vector<int> test_ratings5 = {-1, 2, 5};

    vector<int> expected_rating_ = {2, 2, -2, -2, 2};

    server.AddDocument(1, ""s, DocumentStatus::ACTUAL, test_ratings0);
    server.AddDocument(2, contentsDB[0], DocumentStatus::ACTUAL, test_ratings1);
    server.AddDocument(3, contentsDB[0], DocumentStatus::ACTUAL, test_ratings2);
    server.AddDocument(4, contentsDB[0], DocumentStatus::ACTUAL, test_ratings3);
    server.AddDocument(5, contentsDB[0], DocumentStatus::ACTUAL, test_ratings4);
    server.AddDocument(6, contentsDB[0], DocumentStatus::ACTUAL, test_ratings5);

    for (const Document& document : server.FindTopDocuments(contentsDB[0])) {
        ASSERT_EQUAL(document.rating, expected_rating_[document.id - 2]);
    }
}

// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #7 TestFindDocumentsWithCustomDocumentStatusFilterFunction ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
void TestFindDocumentsWithCustomDocumentStatusFilterFunction() {
    static const int docCount = 3;
    SearchServer server;

    for (int document_id = test_document_id; document_id < test_document_id + docCount; ++document_id) {
        server.AddDocument(document_id, contentsDB[0], DocumentStatus::ACTUAL, test_ratings);
    }

    auto custom_document_filter_function = [](int document_id, DocumentStatus status, int rating) {
        return document_id > test_document_id;
    };

    const auto found_documents = server.FindTopDocuments(contentsDB[0], custom_document_filter_function);
    ASSERT_EQUAL_HINT(found_documents.size(), docCount - 1, "Server found expected number of documents with custom document filter function"s);
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #8 TestFindDocumentsWithDefaultDocumentStatusFilterFunction ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Поиск документов, имеющих заданный статус по умолчанию.
void TestFindDocumentsWithDefaultDocumentStatusFilterFunction() {
    {
        static const int docStatusCount = 4;
        static const int docCount = 9;
        std::map<DocumentStatus, std::vector<int>> expected_documents;

        SearchServer server;

        for (int document_id = 0; document_id < docCount; ++document_id) {
            DocumentStatus status = static_cast<DocumentStatus>(document_id % docStatusCount);
            server.AddDocument(document_id, contentsDB[0], status, test_ratings);
            expected_documents[status].emplace_back(document_id);
        }

        auto check_status = [=](DocumentStatus status, const std::string& status_string,
                                const std::vector<int>& expected_document_indexes) {
            const auto documents = server.FindTopDocuments(contentsDB[0], status);

            std::vector<int> actual_document_indexes;
            std::transform(documents.begin(), documents.end(), std::back_inserter(actual_document_indexes),
                           [](const Document& document) { return document.id; });
            std::sort(actual_document_indexes.begin(), actual_document_indexes.end());

            ASSERT_EQUAL_HINT(documents.size(), expected_document_indexes.size(), "Server found correct number of documents"s);

            ASSERT_HINT(std::equal(actual_document_indexes.begin(), actual_document_indexes.end(), expected_document_indexes.begin()),
                        "Server found correct document ID"s);
        };

        check_status(DocumentStatus::ACTUAL, "ACTUAL", expected_documents[DocumentStatus::ACTUAL]);
        check_status(DocumentStatus::IRRELEVANT, "IRRELEVANT", expected_documents[DocumentStatus::IRRELEVANT]);
        check_status(DocumentStatus::BANNED, "BANNED", expected_documents[DocumentStatus::BANNED]);
        check_status(DocumentStatus::REMOVED, "REMOVED", expected_documents[DocumentStatus::REMOVED]);
    }

    {
        SearchServer server;
        server.AddDocument(test_document_id, contentsDB[0], DocumentStatus::ACTUAL, test_ratings);

        const auto found_documents = server.FindTopDocuments(contentsDB[0]);
        ASSERT_EQUAL(found_documents.size(), 1);

        ASSERT_EQUAL_HINT(found_documents[0].id, test_document_id, "Server found correct document with ACTUAL status and IMPLICIT ACTUAL function argument"s);
    }
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #8 TestStatus ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Поиск документов, имеющих заданный статус.
void TestStatus() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    const int doc_id2 = 43;
    const string content2 = "cat and dog in the home"s;
    const vector<int> ratings2 = {3, 3, 3};

    const int doc_id3 = 44;
    const string content3 = "cat and dog in the black box"s;
    const vector<int> ratings3 = {4, 4, 4};

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id2, content2, DocumentStatus::IRRELEVANT, ratings2);
        server.AddDocument(doc_id3, content3, DocumentStatus::REMOVED, ratings3);
        const auto found_docs = server.FindTopDocuments("cat"s, DocumentStatus::IRRELEVANT);
        ASSERT_EQUAL(found_docs.size(), 1);
        ASSERT_EQUAL(found_docs[0].id, 43);
        const auto found_docs2 = server.FindTopDocuments("cat"s, DocumentStatus::REMOVED);
        ASSERT_EQUAL(found_docs2.size(), 1);
        ASSERT_EQUAL(found_docs2[0].id, 44);
    }
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #9 TestCountingRelevants ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Корректное вычисление релевантности найденных документов
void TestCountingRelevants() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    const int doc_id2 = 43;
    const string content2 = "cat and dog in the home"s;
    const vector<int> ratings2 = {3, 3, 3};

    const int doc_id3 = 44;
    const string content3 = "cat and dog in the black box"s;
    const vector<int> ratings3 = {4, 4, 4};

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id3, content3, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat dog -city"s, DocumentStatus::ACTUAL);
        ASSERT_EQUAL(found_docs.size(), 2);
        ASSERT_EQUAL(found_docs[0].id, 43);
        ASSERT_EQUAL(found_docs[1].id, 44);
    }
}
// =============================================================================

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestAddingDocumentsToTheServer);
    RUN_TEST(TestMatching);
    RUN_TEST(TestSortByRelevance);
    RUN_TEST(TestRatingCalculation);
    RUN_TEST(TestFindDocumentsWithCustomDocumentStatusFilterFunction);
    RUN_TEST(TestFindDocumentsWithDefaultDocumentStatusFilterFunction);
    RUN_TEST(TestStatus);
    RUN_TEST(TestCountingRelevants);
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}