// #define _GLIBCXX_DEBUG 1

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

    // #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& rawQuery,
                                        DocumentPredicate document_predicate,
                                        vector<Document>& matched_documents) const {
        Query parsedQuery;

        bool parseQueryResult = ParseQuery(rawQuery, parsedQuery);
        if (!parseQueryResult || parsedQuery.plus_words.empty()) {
            return false;
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

        return true;
    }

    // #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
    [[nodiscard]] bool FindTopDocuments(const string& rawQuery,
                                        DocumentStatus status,
                                        vector<Document>& matched_documents) const {
        return FindTopDocuments(
            rawQuery,
            [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            },
            matched_documents);
    }

    // #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
    [[nodiscard]] bool FindTopDocuments(const string& rawQuery,
                                        vector<Document>& matched_documents) const {
        return FindTopDocuments(rawQuery, DocumentStatus::ACTUAL, matched_documents);
    }

    // result of MatchDocument(...) is placed into tuple< vector<string>,DocumentStatus >
    // First component of tuple is a vector of PLUS-query-words found in document specified by docID
    // Second component of a tuple is STATUS of document specified by docID
    [[nodiscard]] bool MatchDocument(const string& rawQuery,
                                     int docID,
                                     tuple<vector<string>, DocumentStatus>& result) const {
        Query parsedQuery;  // parsedQuery is represented by 2 sets of string: 1)plus_words and 2)minus_words

        // placeholder for first component (vector<string>) of a "result" tuple  (3rd parameter of a function)
        vector<string> intersectionWithPlusWords;

        // / placeholder for second component (DocumentStatus) of a "result" tuple (3rd parameter of a function)
        DocumentStatus status;

        // return false if document with "docID" isn't found
        if (_documents.count(docID)) {
            status = _documents.at(docID).status;
        } else {
            result = {intersectionWithPlusWords, status};
            return false;
        }

        // return false if:
        // 1) "rawQuery" contains special characters or redundant minuses
        // 2) "plus_words" set of query is empty
        bool parseQueryResult = ParseQuery(rawQuery, parsedQuery);

        if (!parseQueryResult || parsedQuery.plus_words.empty()) {
            result = {intersectionWithPlusWords, status};
            return false;
        }

        // return true although some minus-word presents in document specified by docID,
        // but with empty "intersectionWithPlusWords"
        for (const string& minusWord : parsedQuery.minus_words) {
            // if "minusWord" is found in inverted index
            if (_word_docID_freqs.count(minusWord)) {
                // if "docID" key is found in map "docID : frequency"
                if (_word_docID_freqs.at(minusWord).count(docID)) {
                    result = {intersectionWithPlusWords, status};
                    return true;
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
        result = {intersectionWithPlusWords, status};

        return true;
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

// ++++++++++-------- Реализация макросов и необходимых функций для тестов ----------++++++++++

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
void AssertEqualImpl(const T& t, const U& u,
                     const string& t_str, const string& u_str,
                     const string& file, const string& func,
                     unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
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
// ++++++++++-------- Окончание макросов и необходимых функций для тестов -----------++++++++++

// -------- Начало модульных тестов поисковой системы ----------

void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server(""s);
        (void)server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        vector<Document> result;
        bool res = server.FindTopDocuments("in"s, result);
        ASSERT_EQUAL(res, true);
        ASSERT_EQUAL(result.size(), 1);
        const Document& doc0 = result[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server("in the"s);
        (void)server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        vector<Document> result;
        bool check = server.FindTopDocuments("in"s, result);
        ASSERT_EQUAL(check, true);
        ASSERT(result.empty());
    }
}

void TestAddDocument() {
    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "а"s, DocumentStatus::BANNED, {5, 3, 4, 1});
        (void)server.AddDocument(3, "б"s, DocumentStatus::IRRELEVANT, {5, 3, 4, 1});
        (void)server.AddDocument(4, "с"s, DocumentStatus::REMOVED, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("пропала кошка"s, result);
        ASSERT_EQUAL_HINT(result.size(), 1,
                          "Проверяет на добавление документов с разными статусами"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("пропала кошка"s, result);
        ASSERT_HINT(result[0].relevance == 0 &&
                        result[0].id == 1 &&
                        result[0].rating == 13 / 4,
                    "Проверяет на добавление одного документа и вычисление его рейтинга"s);
    }

    {
        SearchServer server(""s);
        bool check = server.AddDocument(0, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        ASSERT_EQUAL_HINT(check, true,
                          "Проверяет на добавление id == 0"s);
        check = server.AddDocument(-1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        ASSERT_EQUAL_HINT(check, false,
                          "Проверяет на добавление отрицательного id"s);
        check = server.AddDocument(-2, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        ASSERT_EQUAL_HINT(check, false,
                          "Проверяет на добавление отрицательного id"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(0, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(1, "пропала собака"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});

        (void)server.AddDocument(0, "пропала ящерица"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(1, "пропал попугай"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});

        vector<Document> result;
        (void)server.FindTopDocuments("пропала кошка"s, result);
        ASSERT_EQUAL_HINT(result.size(), 2,
                          "Проверяет на добавление документа с существующим id"s);
    }

    {
        SearchServer server(""s);
        ASSERT_EQUAL_HINT(server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2}), false,
                          "Проверяем на добавление документа со спецсимволами"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("кошка пропала"s, result);
        ASSERT_HINT(result.size() == 1 &&
                        result[0].relevance == 0 &&
                        result[0].id == 1 &&
                        result[0].rating == 13 / 4,
                    "Проверяет, что результат не зависит от перемены мест слов"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "пропала собака"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "потерялась лисица"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("лисица -пропала"s, result);
        ASSERT_EQUAL_HINT(result.size(), 1,
                          "Проверяет на исключение документа из списка выдачи"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "лисица пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "собака пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "потерялась лисица"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("лисица --пропала"s, result);
        ASSERT_HINT(check == false &&
                        result.empty(),
                    "Проверяет на обработку --"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(3, "кошка игривая пропала"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(2, "собака злая"s, DocumentStatus::BANNED, {4, 4, 4, 4});
        (void)server.AddDocument(1, "попугай крикливый нашёлся"s, DocumentStatus::REMOVED, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("кошка злая потерялась"s, result);
        ASSERT_HINT(abs(result[0].relevance - 0.3662040) < 1e-5, "Проверяет счёт релевантности");
        ASSERT_EQUAL_HINT(result[0].rating, 4, "Проверяет счёт рейтинга");
        ASSERT_HINT(result.size() == 1 && result[0].id == 3, "Проверяет на работу фильтра"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "кошка игривая пропала"s, DocumentStatus::BANNED, {1, 2, 3, 4});
        (void)server.AddDocument(1, "кошка злая потерялась"s, DocumentStatus::BANNED, {4, 4, 4, 4});
        vector<Document> result;
        (void)server.FindTopDocuments("кошка злая потерялась"s, DocumentStatus::BANNED, result);
        ASSERT_HINT(result.size() == 1 &&
                        result[0].rating == 2,
                    "Проверяет на изменение данных, при добавлении документов с одним и тем же индексом"s);
    }

    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(4, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(5, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(6, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(7, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("пропала кошка"s, result);
        ASSERT_EQUAL_HINT(result.size(), 5,
                          "Проверяет на огранчение по количеству в топе"s);
    }

    {
        SearchServer server("на c"s);
        (void)server.AddDocument(1, "пропался кот"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "пропала собака"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "потерялась лиса"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("кот - - - - "s, result);
        ASSERT_HINT(check == false &&
                        result.size() == 0,
                    "проверка на пустой -"s);
    }

    {
        SearchServer server("и на c"s);
        (void)server.AddDocument(1, "и кот попался "s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "и собака пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "лиса пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("лиса -пропала и"s, result);
        ASSERT_EQUAL(check, true);
        ASSERT_EQUAL_HINT(result.size(), 0,
                          "Поиск стоп-слова должен вернуть пустой результат"s);
    }

    {
        SearchServer server("один"s);
        (void)server.AddDocument(1, "и кот попался "s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "и собака пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "лиса пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(4, "один"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("один"s, result);
        ASSERT_EQUAL(check, true);
        ASSERT_EQUAL_HINT(result.size(), 0,
                          "Поиск стоп-слова должен вернуть пустой результат"s);
    }
}

void TestSetStopWords() {
    {
        SearchServer server("по и или на с до за не"s);

        (void)server.AddDocument(1, "машина с беспилотным управлением и печкой"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(2, "автомобиль пропал"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});

        vector<Document> result;
        (void)server.FindTopDocuments("машина"s, result);
        ASSERT(abs(result[0].relevance - 0.17328679) < 1e-6);
    }
}

void TestMatchDocument() {
    {
        SearchServer server("и в на белая"s);

        (void)server.AddDocument(1, "машина беспилотная"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(2, "автомобиль пропал"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(3, "машина пропала белая"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});

        tuple<vector<string>, DocumentStatus> result;
        bool res1 = server.MatchDocument("машина -беспилотная"s, 1, result);
        ASSERT_EQUAL(res1, true);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res2 = server.MatchDocument("машина -беспилотная"s, 2, result);
        ASSERT_EQUAL(res2, true);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res3 = server.MatchDocument("и машина -беспилотная пропала на"s, 3, result);
        ASSERT_EQUAL(res3, true);
        ASSERT_EQUAL(get<0>(result).size(), 2);

        bool res4 = server.MatchDocument("машина -беспилотная пропала"s, 4, result);
        ASSERT_EQUAL(res4, false);

        // запрос состоит из одного стоп-слова
        bool res5 = server.MatchDocument("на"s, 3, result);
        ASSERT_EQUAL(res5, true);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res6 = server.MatchDocument("машина --беспилотная пропала"s, 3, result);
        ASSERT_EQUAL(res6, false);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool check = server.MatchDocument("и машина -беспилотная -пропала"s, 3, result);
        ASSERT_EQUAL(check, true);
        ASSERT_EQUAL_HINT(get<0>(result).size(), 0,
                          "Поиск стоп-слова должен вернуть пустой результат"s);
    }
}

void TestSortRelevance() {
    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "робот утилизатор"s, DocumentStatus::ACTUAL, {5, 5, 5, 5, 5});
        (void)server.AddDocument(2, "беспилотный самолёт"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(3, "новая посудомойка"s, DocumentStatus::ACTUAL, {3, 3, 3});
        (void)server.AddDocument(4, "робот пылесос"s, DocumentStatus::ACTUAL, {2, 2});

        vector<Document> result;
        (void)server.FindTopDocuments("робот утилизатор беспилотный"s, result);
        ASSERT(result.size() == 3 &&
               result[0].id == 1 &&
               result[1].id == 2 &&
               result[2].id == 4);

        (void)server.AddDocument(5, "робот робот"s, DocumentStatus::ACTUAL, {1});

        vector<Document> result1;
        (void)server.FindTopDocuments("робот утилизатор беспилотный"s, result1);
        ASSERT(result1.size() == 4 &&
               result1[0].id == 1 &&
               result1[1].id == 2 &&
               result1[2].id == 5 &&
               result1[3].id == 4);

        vector<Document> result2;
        (void)server.FindTopDocuments(
            "робот утилизатор беспилотный"s, [](int document_id, DocumentStatus status, int rating) {
                return rating > 3;
            },
            result2);
        ASSERT_EQUAL(result2.size(), 2);
    }
}

void TestSplitIntoWords() {
    auto vec1 = SplitIntoWords(""s);
    ASSERT_EQUAL(vec1.size(), 0);
    auto vec2 = SplitIntoWords("раз два три"s);
    ASSERT(vec2.size() == 3 &&
           vec2[0] == "раз"s);
    auto vec3 = SplitIntoWords("     раз      два      три     "s);
    ASSERT(vec3.size() == 3 &&
           vec3[0] == "раз"s);
}

void TestGetDocumentId() {
    SearchServer server(""s);
    (void)server.AddDocument(0, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
    (void)server.AddDocument(1, "а"s, DocumentStatus::BANNED, {5, 3, 4, 1});
    (void)server.AddDocument(2, "б"s, DocumentStatus::IRRELEVANT, {5, 3, 4, 1});
    (void)server.AddDocument(3, "с"s, DocumentStatus::REMOVED, {5, 3, 4, 1});
    ASSERT_EQUAL(server.GetDocumentId(-1), SearchServer::INVALID_DOCUMENT_ID);
    ASSERT_EQUAL(server.GetDocumentId(5), SearchServer::INVALID_DOCUMENT_ID);
    ASSERT_EQUAL(server.GetDocumentId(2), 2);
    ASSERT_EQUAL(server.GetDocumentId(3), 3);
}

/* void TestIsValidWord(){
    ASSERT_EQUAL(SearchServer::IsValidWord("--пушистый"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("------"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("-"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("\0"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("иван-чай"), true);
} */

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestAddDocument);
    RUN_TEST(TestSetStopWords);
    RUN_TEST(TestMatchDocument);
    RUN_TEST(TestSortRelevance);
    RUN_TEST(TestSplitIntoWords);
    RUN_TEST(TestGetDocumentId);

    // RUN_TEST(TestIsValidWord);
}

// --------- Окончание модульных тестов поисковой системы -----------

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

// code proposed as an example for the task
int example_1() {
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

    return 0;
}

// my example
int example_2() {
    SearchServer search_server("и в на"s);

    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void)search_server.AddDocument(0, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }

    if (!search_server.AddDocument(2, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }

    if (!search_server.AddDocument(5, "большой пёс скворец"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }

    vector<Document> documents;
    if (search_server.FindTopDocuments("пушистый пёс"s, documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
    int number = 3;
    cout << "Идентификатор документа с порядковым номером " << number << " -> " << search_server.GetDocumentId(number) << endl;

    return 0;
}

int main() {
#ifdef _GLIBCXX_DEBUG
    TestSearchServer();
    cerr << "Search server testing finished"s << endl;
#endif  //_GLIBCXX_DEBUG
    example_1();

    // example_2();
}