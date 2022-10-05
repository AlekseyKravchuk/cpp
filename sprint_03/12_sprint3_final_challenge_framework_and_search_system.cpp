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

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

struct Document {
    int id;
    double relevance;
    int rating;
};

bool isEqualRelevances(double relevance_1, double relevance_2) {
    const double REASONABLE_ERROR = 1e-6;
    if (abs(relevance_1 - relevance_2) < REASONABLE_ERROR) {
        return true;
    } else {
        return false;
    }
}

bool operator==(const Document& lhsDoc, const Document& rhsDoc) {
    if (lhsDoc.id == rhsDoc.id && isEqualRelevances(lhsDoc.relevance, rhsDoc.relevance) && lhsDoc.rating == rhsDoc.rating) {
        return true;
    } else {
        return false;
    }
}

bool operator!=(const Document& lhsDoc, const Document& rhsDoc) {
    return !(operator==(lhsDoc, rhsDoc));
}

ostream& operator<<(ostream& os, const Document& doc) {
    os << "{"s;
    os << "docID = "s << doc.id << "; "s;
    os << "relevance = "s << doc.relevance << "; "s;
    os << "rating = "s << doc.rating << ";"s;
    os << "}"s;

    return os;
}

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
    // 10^(-6); constant used when comparing two document relevances (of type "double")
    const double _REASONABLE_ERROR = 1e-6;

    int GetDocumentCount() const {
        return _documents.size();
    }

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stopWords.insert(word);
        }
    }

    void AddDocument(int docID,
                     const string& document,
                     DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double invertedWordCount = 1.0 / words.size();

        for (const string& word : words) {
            _word2docID2freqs[word][docID] += invertedWordCount;
        }
        _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});
    }

    // #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
    template <typename Predicate>
    vector<Document> FindTopDocuments(const string& rawQuery, Predicate pred) const {
        const Query query = ParseQuery(rawQuery);
        auto matchedDocuments = FindAllDocuments(query, pred);

        sort(matchedDocuments.begin(), matchedDocuments.end(),
             [this](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < this->_REASONABLE_ERROR) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matchedDocuments.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matchedDocuments.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matchedDocuments;
    }

    // #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
    vector<Document> FindTopDocuments(const string& rawQuery,
                                      DocumentStatus externalStatus) const {
        return FindTopDocuments(rawQuery,
                                [externalStatus](int document_id, DocumentStatus status, int rating) { return externalStatus == status; });
    }

    // #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
    vector<Document> FindTopDocuments(const string& rawQuery) const {
        return FindTopDocuments(rawQuery, DocumentStatus::ACTUAL);
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& rawQuery,
                                                        int document_id) const {
        const Query query = ParseQuery(rawQuery);
        vector<string> matchedWords;
        for (const string& word : query.plusWords) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }
            if (_word2docID2freqs.at(word).count(document_id)) {
                matchedWords.push_back(word);
            }
        }
        for (const string& word : query.minusWords) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }
            if (_word2docID2freqs.at(word).count(document_id)) {
                matchedWords.clear();
                break;
            }
        }
        return {matchedWords, _documents.at(document_id).status};
    }

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> _stopWords;
    map<string, map<int, double>> _word2docID2freqs;
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
        return (ratings.empty()) ? 0 : reduce(ratings.begin(), ratings.end(), 0.0) / static_cast<double>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool isMinusWord;
        bool isStopWord;
    };

    QueryWord ParseQueryWord(string text) const {
        bool isMinusWord = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            isMinusWord = true;
            text = text.substr(1);
        }
        return {text, isMinusWord, IsStopWord(text)};
    }

    struct Query {
        set<string> plusWords;
        set<string> minusWords;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord queryWord = ParseQueryWord(word);
            if (!queryWord.isStopWord) {
                if (queryWord.isMinusWord) {
                    query.minusWords.insert(queryWord.data);
                } else {
                    query.plusWords.insert(queryWord.data);
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

        for (const string& word : query.plusWords) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }

            const double inverseDocumentFreq = ComputeWordInverseDocumentFreq(word);
            for (const auto [docID, term_freq] : _word2docID2freqs.at(word)) {
                if (pred(docID, _documents.at(docID).status, _documents.at(docID).rating)) {
                    doc2relevance[docID] += term_freq * inverseDocumentFreq;
                }
            }
        }

        for (const string& word : query.minusWords) {
            if (_word2docID2freqs.count(word) == 0) {
                continue;
            }
            for (const auto [docID, _] : _word2docID2freqs.at(word)) {
                doc2relevance.erase(docID);
            }
        }

        vector<Document> matched_documents;
        for (const auto [docID, relevance] : doc2relevance) {
            matched_documents.push_back(
                {docID, relevance, _documents.at(docID).rating});
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

ostream& operator<<(ostream& out, const vector<string>& container) {
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first) {
            out << ", "s;
        }
        is_first = false;
        out << element;
    }
    return out;
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
    /* 03 */ "dog sits on bed"s,
    /* 04 */ "dog was walking with his master at the street"s,
    /* 05 */ "lovely dog with big curly tail",
    /* 06 */
    "      "s
    /* 07 */ "earth worm"s
    /* ,"extra document to fail test"s */};

const std::vector<int> testRatings = {3, 0, 0, 0};
constexpr int testDocID = 1;

// supplementaty funciton to compose documents DB from contentsDB
void ComposeDocumentsFromContentsDB(SearchServer& server) {
    for (size_t docID = 0; docID < contentsDB.size(); ++docID) {
        server.AddDocument(docID, contentsDB[docID], DocumentStatus::ACTUAL, testRatings);
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
        const auto foundDocuments = server.FindTopDocuments(word);
        ASSERT(foundDocuments.size() > 0);
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
        const auto foundDocuments = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(foundDocuments.size(), 1u);
        const Document& doc0 = foundDocuments[0];
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

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #3 TestMinusWordsExcludedFromQuery ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Поддержка минус-слов. Документы, содержащие минус-слова из поискового запроса, не должны включаться в результаты поиска.
void TestMinusWordsExcludedFromQuery() {
    SearchServer server;
    ComposeDocumentsFromContentsDB(server);

    auto foundDocuments = server.FindTopDocuments("dog -walking"s);
    ASSERT_EQUAL_HINT(foundDocuments.size(), 4, "Server returns ONLY the documents without minus words, which were in query"s);

    // test case when PLUS-words and MINUS-words intersect
    foundDocuments = server.FindTopDocuments("dog walking joyfully -walking -plays");
    ASSERT_EQUAL(foundDocuments.size(), 3);

    foundDocuments = server.FindTopDocuments("dog walking -walking -likes -bed  -tail");
    ASSERT_EQUAL(foundDocuments.size(), 1);
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #4 TestMatching ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Соответствие документов поисковому запросу. При этом должны быть возвращены все слова из поискового запроса, присутствующие в документе.
// Если есть пересечение хотя бы по одному минус-слову, должен возвращаться пустой список слов.
void TestMatching() {
    SearchServer server;
    ComposeDocumentsFromContentsDB(server);

    for (size_t docID = 0; docID < contentsDB.size(); ++docID) {
        // check if empty string will be found
        auto [wordsItersection, _] = server.MatchDocument(""s, docID);
        ASSERT_HINT(wordsItersection.empty(), "matching empty word for docID"s);

        // check if MatchDocument finds words not present in the document
        std::tie(wordsItersection, _) = server.MatchDocument("words not existing in document"s, docID);
        ASSERT_HINT(wordsItersection.empty(), "words not present in the document was found"s);

        // check if MatchDocument finds all the intersection of PLUS-words and QUERY-words
        int docID2test = 4;
        if (docID == docID2test) {
            std::tie(wordsItersection, _) = server.MatchDocument("dog street master"s, docID2test);
            ASSERT_EQUAL_HINT(wordsItersection.size(), 3, "matching existing words for docid"s);
            const vector<string> wordsIntersection1 = {"dog"s, "master"s, "street"s};
            ASSERT_EQUAL_HINT(wordsItersection, wordsIntersection1, "matching existing words for docid"s);

            std::tie(wordsItersection, _) = server.MatchDocument("dog -walking"s, docID2test);
            ASSERT_EQUAL_HINT(wordsItersection.size(), 0, "no matching minus words for docid"s);
            const vector<string> wordsIntersection2 = {};
            ASSERT_EQUAL_HINT(wordsItersection, wordsIntersection2, "matching existing words for docid"s);
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

    server.AddDocument(0, "dog"s, DocumentStatus::ACTUAL, testRatings);
    server.AddDocument(1, "dog was"s, DocumentStatus::ACTUAL, testRatings);
    server.AddDocument(2, "dog was happy"s, DocumentStatus::ACTUAL, testRatings);
    server.AddDocument(3, "dog was happy to"s, DocumentStatus::ACTUAL, testRatings);
    server.AddDocument(4, "dog was happy to walk"s, DocumentStatus::ACTUAL, testRatings);
    server.AddDocument(5, "dog was happy to walk in"s, DocumentStatus::ACTUAL, testRatings);
    server.AddDocument(6, "dog was happy to walk in park"s, DocumentStatus::ACTUAL, testRatings);

    const auto foundDocuments = server.FindTopDocuments("happy dog park walk"s);
    const std::vector<int> expectedDocumentIds = {6, 4, 5, 2, 0};

    ASSERT_EQUAL(server.GetDocumentCount(), 7);

    ASSERT_EQUAL(foundDocuments.size(), expectedDocumentIds.size());

    bool isSortedByRelevance =
        std::is_sorted(foundDocuments.begin(), foundDocuments.end(),
                       [](const Document& left, const Document& right) { return left.relevance > right.relevance; });

    ASSERT_HINT(isSortedByRelevance, "sorted by relevance"s);
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #6 TestRatingCalculation ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
void TestRatingCalculation() {
    SearchServer server;

    vector<int> testRatings0 = {3, 0, 0, 0};
    vector<int> testRatings1 = {1, 2, 4};
    vector<int> testRatings2 = {1, 2, 5};
    vector<int> testRatings3 = {-1, -2, -4};
    vector<int> testRatings4 = {-1, -2, -5};

    vector<int> expectedRating = {(3 + 0 + 0 + 0) / 4,
                                  (1 + 2 + 4) / 3,
                                  (1 + 2 + 5) / 3,
                                  (-1 - 2 - 4) / 3,
                                  (-1 - 2 - 5) / 3};

    // contentsDB[0] equals to "happy dog walking joyfully"s;
    server.AddDocument(0, contentsDB[0], DocumentStatus::ACTUAL, testRatings0);
    server.AddDocument(1, contentsDB[0], DocumentStatus::ACTUAL, testRatings1);
    server.AddDocument(2, contentsDB[0], DocumentStatus::ACTUAL, testRatings2);
    server.AddDocument(3, contentsDB[0], DocumentStatus::ACTUAL, testRatings3);
    server.AddDocument(4, contentsDB[0], DocumentStatus::ACTUAL, testRatings4);

    const auto foundDocuments = server.FindTopDocuments(contentsDB[0]);

    for (size_t i; i < MAX_RESULT_DOCUMENT_COUNT; ++i) {
        int correctIndex = foundDocuments[i].id;
        ASSERT_EQUAL(foundDocuments[i].rating, expectedRating[correctIndex]);
    }
}

// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #7 TestFindDocumentsWithCustomDocumentStatusFilterFunction ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
void TestFindDocumentsWithCustomDocumentStatusFilterFunction() {
    static const int docCount = 3;
    SearchServer server;

    for (int docID = testDocID; docID < testDocID + docCount; ++docID) {
        server.AddDocument(docID, contentsDB[0], DocumentStatus::ACTUAL, testRatings);
    }

    auto customDocumentFilterFunction = [](int docID, DocumentStatus status, int rating) {
        return docID > testDocID;
    };

    const auto foundDocuments = server.FindTopDocuments(contentsDB[0], customDocumentFilterFunction);
    ASSERT_EQUAL_HINT(foundDocuments.size(), docCount - 1, "Server found expected number of documents with custom document filter function"s);
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #8 TestFindDocumentsWithDefaultDocumentStatusFilterFunction ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Поиск документов, имеющих заданный статус по умолчанию.
void TestFindDocumentsWithDefaultDocumentStatusFilterFunction() {
    {
        SearchServer server;
        server.AddDocument(0, "dog was walking with his master at the street"s, DocumentStatus::ACTUAL, {3, 0, 0, 0});
        server.AddDocument(1, "dog likes walk and run"s, DocumentStatus::IRRELEVANT, {3, 0, 0, 0});
        server.AddDocument(2, "happy dog walking joyfully"s, DocumentStatus::BANNED, {3, 0, 0, 0});
        server.AddDocument(3, "happy dog walking joyfully"s, DocumentStatus::REMOVED, {3, 0, 0, 0});
        server.AddDocument(4, "happy dog walking joyfully"s, DocumentStatus::IRRELEVANT, {3, 0, 0, 0});
        server.AddDocument(5, "dog sits on bed"s, DocumentStatus::ACTUAL, {3, 0, 0, 0});

        const auto foundDocuments = server.FindTopDocuments("dog sits"s);
        ASSERT_EQUAL(foundDocuments.size(), 2u);
        ASSERT_EQUAL_HINT(foundDocuments[0].id, 5, "Server found correct document with ACTUAL status and IMPLICIT ACTUAL function argument"s);
        ASSERT_EQUAL_HINT(foundDocuments[1].id, 0, "Server found correct document with ACTUAL status and IMPLICIT ACTUAL function argument"s);
    }
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #9 TestStatus ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Поиск документов, имеющих заданный статус.
void TestStatus() {

    {
        SearchServer server;
        server.AddDocument(42, "cat in the city"s,              DocumentStatus::ACTUAL,     {1, 2, 3});
        server.AddDocument(43, "cat and dog in the home"s,      DocumentStatus::IRRELEVANT, {3, 3, 3});
        server.AddDocument(44, "cat and dog in the black box"s, DocumentStatus::REMOVED,    {4, 4, 4});

        const auto foundDocuments1 = server.FindTopDocuments("cat"s, DocumentStatus::IRRELEVANT);
        ASSERT_EQUAL(foundDocuments1.size(), 1u);
        ASSERT_EQUAL(foundDocuments1[0].id, 43);

        const auto foundDocuments2 = server.FindTopDocuments("cat"s, DocumentStatus::REMOVED);
        ASSERT_EQUAL(foundDocuments2.size(), 1);
        ASSERT_EQUAL(foundDocuments2[0].id, 44);
    }
}
// =============================================================================

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ #10 TestRelevanceCalculation ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Корректное вычисление релевантности найденных документов
void TestRelevanceCalculation() {
    {
        SearchServer server;
        server.AddDocument(42, "cat in the city"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(43, "cat and dog in the home"s, DocumentStatus::ACTUAL, {3, 3, 3});
        server.AddDocument(44, "cat and dog in the black box"s, DocumentStatus::ACTUAL, {4, 4, 4});
        const auto foundDocuments = server.FindTopDocuments("cat dog -city"s, DocumentStatus::ACTUAL);
        ASSERT_EQUAL(foundDocuments.size(), 2);

        const Document doc0 = {43, 0.067577, 3};
        const Document docFound0 = foundDocuments[0];

        const Document doc1 = {44, 0.057923, 4};
        const Document docFound1 = foundDocuments[1];

        ASSERT_EQUAL(docFound0, doc0);
        ASSERT_EQUAL(docFound1, doc1);
    }
}
// =============================================================================

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    /* 1 */ RUN_TEST(TestAddingDocumentsToTheServer);
    /* 2 */ RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    /* 3 */ RUN_TEST(TestMinusWordsExcludedFromQuery);
    /* 4 */ RUN_TEST(TestMatching);
    /* 5 */ RUN_TEST(TestSortByRelevance);
    /* 6 */ RUN_TEST(TestRatingCalculation);
    /* 7 */ RUN_TEST(TestFindDocumentsWithCustomDocumentStatusFilterFunction);
    /* 8 */ RUN_TEST(TestFindDocumentsWithDefaultDocumentStatusFilterFunction);
    /* 9 */ RUN_TEST(TestStatus);
    /* 10 */ RUN_TEST(TestRelevanceCalculation);
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();

    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}