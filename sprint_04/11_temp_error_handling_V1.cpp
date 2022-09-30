// search_server_s3_t1_v1.cpp


// ======================= Информация для поиска бага ===============================
/*
Если в документе в MatchDocument не найдено ни одного слова, это не значит, что в запросе ошибка
*/
// ==================================================================================

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

template <typename Function>
void RunTestImpl(Function function, const string& func) {
    function();
    cerr << func << " OK"s << endl;
}

#define RUN_TEST(func) RunTestImpl((func), #func)

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const pair<Key, Value>& container) {
    out << container.first << ": " << container.second;
    return out;
}

template <typename Container>
void Print(ostream& out, const Container& container) {
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first) {
            out << ", "s;
        }
        is_first = false;
        out << element;
    }
}

template <typename Element>
ostream& operator<<(ostream& out, const vector<Element>& container) {
    out << "["s;
    Print(out, container);
    out << "]"s;
    return out;
}

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;
    return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;
    return out;
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

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str,
                const string& file, const string& func,
                unsigned line, const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "Assert("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

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

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id), relevance(relevance), rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

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
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    }

    explicit SearchServer(const string& stop_words_text)
        : SearchServer(SplitIntoWords(stop_words_text))  // Invoke delegating constructor from string container
    {
    }

    // Defines an invalid document id
    // You can refer to this constant as SearchServer::INVALID_DOCUMENT_ID
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    [[nodiscard]] bool AddDocument(int document_id, const string& document, DocumentStatus status,
                                   const vector<int>& ratings) {
        if ((document_id < 0) || (documents_.count(document_id) > 0)) {
            return false;
        }
        const vector<string> words = SplitIntoWordsNoStop(document);
        for (const string& word : words) {
            if (!IsValidWord(word)) {
                return false;
            }
        }
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
        return true;
    }

    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate,
                                        vector<Document>& result) const {
        for (auto word : SplitIntoWords(raw_query)) {
            if (!IsStopWord(word)) {
                break;
            } else {
                continue;
            }
            result = {};
            return false;
        }
        const Query query = ParseQuery(raw_query);
        if (!query.minus_words.empty()) {
            for (string word : query.minus_words) {
                QueryWord query_word = ParseQueryWord(word);
                if (query_word.is_minus || query_word.data.empty()) {
                    result = {};
                    return false;
                }
                if (query_word.data[0] == 32) {
                    result = {};
                    return false;
                }
                if (!IsValidWord(word)) {
                    result = {};
                    return false;
                }
            }
        }
        if (!query.plus_words.empty()) {
            for (string word : query.plus_words) {
                if (!IsValidWord(word)) {
                    result = {};
                    return false;
                }
            }

        } else {
            result = {};
            return false;
        }

        auto matched_documents = FindAllDocuments(query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
            if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }

        if (!(matched_documents.size() > 0)) {
            result = {};
            return false;
        }
        result = matched_documents;
        return true;
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentStatus status,
                                        vector<Document>& result) const {
        return FindTopDocuments(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) { return document_status == status; }, result);
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, vector<Document>& result) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL, result);
    }

    [[nodiscard]] bool MatchDocument(const string& raw_query, int document_id,
                                     tuple<vector<string>, DocumentStatus>& result) const {
        for (auto word : SplitIntoWords(raw_query)) {
            if (!IsStopWord(word)) {
                break;
            } else {
                continue;
            }
            result = {};
            return false;
        }
        const Query query = ParseQuery(raw_query);
        if (!(documents_.count(document_id) > 0)) {
            result = {};
            return false;
        }
        if (!query.minus_words.empty()) {
            for (string word : query.minus_words) {
                QueryWord query_word = ParseQueryWord(word);
                if (query_word.is_minus || query_word.data.empty()) {
                    result = {};
                    return false;
                }
                if (query_word.data[0] == 32) {
                    result = {};
                    return false;
                }
                if (!IsValidWord(word)) {
                    result = {};
                    return false;
                }
            }
        }
        if (!query.plus_words.empty()) {
            for (string word : query.plus_words) {
                if (!IsValidWord(word)) {
                    result = {};
                    return false;
                }
                QueryWord query_word = ParseQueryWord(word);
                if (query_word.is_stop) {
                    result = {};
                    return false;
                }
            }
        }
        vector<string> matched_words;

        if (!query.plus_words.empty()) {
            for (const auto& minus_word : query.minus_words) {
                if (word_to_document_freqs_.count(minus_word)) {
                    if (word_to_document_freqs_.at(minus_word).count(document_id)) {
                        result = {};
                        return false;
                    }
                }
            }

            for (const string& word : query.plus_words) {
                if (word_to_document_freqs_.count(word) == 0) {
                    continue;
                }
                if (word_to_document_freqs_.at(word).count(document_id)) {
                    matched_words.push_back(word);
                }
            }
            for (const string& word : query.minus_words) {
                if (word_to_document_freqs_.count(word) == 0) {
                    continue;
                }
                if (word_to_document_freqs_.at(word).count(document_id)) {
                    matched_words.clear();
                    break;
                }
            }
            if (!(matched_words.size() > 0)) {
                result = {};
                return false;
            }
            result = {matched_words, documents_.at(document_id).status};
        }

        return true;
    }

    int GetDocumentId(int index) const {
        if (index > documents_.size()) {
            return SearchServer::INVALID_DOCUMENT_ID;
        } else {
            auto it = documents_.begin();
            std::advance(it, index);
            return it->first;
        }
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

   private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
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
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }
    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }
        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

// ==================== для примера =========================
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server("one"s);
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
        ASSERT_EQUAL(server.FindTopDocuments("in"s, result), false);
        ASSERT(result.empty());
    }
}

void TestAddDocument() {
    {
        SearchServer server(""s);
        (void)server.AddDocument(1, "lost cat"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "a"s, DocumentStatus::BANNED, {5, 3, 4, 1});
        (void)server.AddDocument(3, "b"s, DocumentStatus::IRRELEVANT, {5, 3, 4, 1});
        (void)server.AddDocument(4, "c"s, DocumentStatus::REMOVED, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("lost one cat"s, result);
        ASSERT_EQUAL_HINT(result.size(), 1,
                          "Проверяет на добавление документов c разными статусами"s);
    }

    {
        SearchServer server("one"s);
        (void)server.AddDocument(1, "lost cat"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("lost cat"s, result);
        ASSERT_HINT(result[0].relevance == 0 &&
                        result[0].id == 1 &&
                        result[0].rating == 13 / 4,
                    "Проверяет на добавление одного документа и вычисление его рейтинга"s);
    }

    {
        SearchServer server("one"s);
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
        SearchServer server("one"s);
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
        SearchServer server("one"s);
        ASSERT_EQUAL_HINT(server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2}), false,
                          "Проверяем на добавление документа со спецсимволами"s);
    }

    {
        SearchServer server("one"s);
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
        SearchServer server("one"s);
        (void)server.AddDocument(1, "пропала кошка"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "пропала собака"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "потерялась лисица"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        (void)server.FindTopDocuments("лисица -пропала"s, result);
        ASSERT_EQUAL_HINT(result.size(), 1,
                          "Проверяет на исключение документа из списка выдачи"s);
    }

    {
        SearchServer server("one"s);
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
        SearchServer server("one"s);
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
        SearchServer server("one"s);
        (void)server.AddDocument(1, "кошка игривая пропала"s, DocumentStatus::BANNED, {1, 2, 3, 4});
        (void)server.AddDocument(1, "кошка злая потерялась"s, DocumentStatus::BANNED, {4, 4, 4, 4});
        vector<Document> result;
        (void)server.FindTopDocuments("кошка злая потерялась"s, DocumentStatus::BANNED, result);
        ASSERT_HINT(result.size() == 1 &&
                        result[0].rating == 2,
                    "Проверяет на изменение данных, при добавлении документов с одним и тем же индексом"s);
    }

    {
        SearchServer server("one"s);
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
        (void)server.AddDocument(1, "пропался-кот"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "пропала собака"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "потерялась лиса"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("кот - - - - "s, result);
        ASSERT_HINT(check == false &&
                        result.size() == 0,
                    "проверка на пустой -"s);
        check = server.FindTopDocuments("пропался-кот"s, result);
        ASSERT_HINT(check == true &&
                        result.size() == 1,
                    "проверка на пустой -"s);
    }

    {
        SearchServer server("и на c"s);
        (void)server.AddDocument(1, "и кот попался "s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(2, "и собака пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        (void)server.AddDocument(3, "лиса потерялась"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("и на c"s, result);
        ASSERT_EQUAL(check, false);
        ASSERT_EQUAL_HINT(result.size(), 0,
                          "Поиск стоп-слова должен вернуть пустой результат"s);
    }
}

void TestSetStopWords() {
    {
        SearchServer server("the is and stop---word"s);

        (void)server.AddDocument(1, "the car is clean but old"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(2, "these cars are new", DocumentStatus::ACTUAL, {4, 4, 4, 4});

        vector<Document> result1;
        (void)server.FindTopDocuments("car"s, result1);
        ASSERT(abs(result1[0].relevance - 0.17328679) < 1e-6);

        vector<Document> result2;
        (void)server.FindTopDocuments("is"s, result2);
        ASSERT(result2.empty());

        vector<Document> result3;
        (void)server.FindTopDocuments("the is and"s, result3);
        ASSERT(result3.empty());

        vector<Document> result4;
        (void)server.FindTopDocuments("new stop---word"s, result4);
        ASSERT(!result4.empty());
    }
}

void TestMatchDocument() {
    {
        SearchServer server("и в на"s);

        (void)server.AddDocument(1, "машина беспилотная"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(2, "автомобиль пропал"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(3, "машина пропала белая"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(4, "иван-чай"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});

        tuple<vector<string>, DocumentStatus> result;
        bool res1 = server.MatchDocument("машина -беспилотная"s, 1, result);
        ASSERT_EQUAL(res1, false);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res2 = server.MatchDocument("машина -беспилотная"s, 2, result);
        ASSERT_EQUAL(res2, false);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res3 = server.MatchDocument("машина -беспилотная пропала"s, 3, result);
        ASSERT_EQUAL(res3, true);
        ASSERT_EQUAL(get<0>(result).size(), 2);

        bool res4 = server.MatchDocument("машина -беспилотная пропала"s, 5, result);
        ASSERT_EQUAL(res4, false);

        bool res5 = server.MatchDocument("иван-чай"s, 4, result);
        ASSERT_EQUAL(res5, true);

        bool res6 = server.MatchDocument("иван -чай"s, 4, result);
        ASSERT_EQUAL(res6, false);

        bool check = server.MatchDocument("и машина -беспилотная пропала"s, 3, result);
        ASSERT_EQUAL(check, true);
        ASSERT_EQUAL_HINT(get<0>(result).size(), 2,
                          "Поиск стоп-слова должен вернуть пустой результат"s);
    }
}

void TestSortRelevance() {
    {
        SearchServer server("one"s);
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
    auto vec2 = SplitIntoWords("one two three"s);
    ASSERT(vec2.size() == 3 &&
           vec2[0] == "one"s);
    auto vec3 = SplitIntoWords("     one      two      three     "s);
    ASSERT(vec3.size() == 3 &&
           vec3[0] == "one"s);
}

void TestGetDocumentId() {
    SearchServer server(""s);
    (void)server.AddDocument(0, "lost cat"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
    (void)server.AddDocument(1, "a"s, DocumentStatus::BANNED, {5, 3, 4, 1});
    (void)server.AddDocument(2, "b"s, DocumentStatus::IRRELEVANT, {5, 3, 4, 1});
    (void)server.AddDocument(3, "c"s, DocumentStatus::REMOVED, {5, 3, 4, 1});
    ASSERT_EQUAL(server.GetDocumentId(-1), SearchServer::INVALID_DOCUMENT_ID);
    ASSERT_EQUAL(server.GetDocumentId(5), SearchServer::INVALID_DOCUMENT_ID);
    ASSERT_EQUAL(server.GetDocumentId(2), 2);
    ASSERT_EQUAL(server.GetDocumentId(3), 3);
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestAddDocument);
    RUN_TEST(TestSetStopWords);
    RUN_TEST(TestMatchDocument);
    RUN_TEST(TestSortRelevance);
    RUN_TEST(TestSplitIntoWords);
    RUN_TEST(TestGetDocumentId);
}
void test() {
    SearchServer search_server("и в на"s);
    assert((search_server.AddDocument(1, "this is a special char\ncter"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(2, "this is a special \t character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(3, "this is a special character\v"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(4, "this\b is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(5, "this \ris a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\fthis is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\athis is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\0 this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\1 this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\2 this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\3hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\4hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\5hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\6hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\7hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\10hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\11hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\12hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\13hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\14hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\15hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\16hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\17hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\20hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\21hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\22hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\23hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\24hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
    assert((search_server.AddDocument(6, "\25hhh this is a special character"s, DocumentStatus::ACTUAL, {1, 2})) == false);
}

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    bool test_enabled = true;
    if (test_enabled) {
        test();
        TestSearchServer();
    } else {
        SearchServer search_server("и в на"s);

        // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
        // о неиспользуемом результате его вызова
        (void)search_server.AddDocument(1, "No special characters in this text"s, DocumentStatus::ACTUAL, {7, 2, 7});

        if (!search_server.AddDocument(1, "No special characters in this text"s, DocumentStatus::ACTUAL, {1, 2})) {
            cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
        }

        if (!search_server.AddDocument(-1, "No special characters in this text"s, DocumentStatus::ACTUAL, {1, 2})) {
            cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
        }

        if (!search_server.AddDocument(3, "There is a special char\vacter in this text"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
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
}