// ================= Подсказка к решению =================
/*
Вроде как, этот случай я обрабатываю в ParseQuery.
Не могли бы дать подсказку по исправлению кода,
а лучше какой-нибудь пример запроса, который не выдаст пустой результат.

Это тот случай, когда поисковый запрос состоит из одного слова - стоп-слова.
Тут немного некорректная подсказка - результат должен не быть пустым,
а само возвращаемое значение должно быть true, ведь запрос был корректным
*/

#define _GLIBCXX_DEBUG 1
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

vector<int> TakeEvens(const vector<int>& numbers) {
    vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}

map<string, int> TakeAdults(const map<string, int>& people) {
    map<string, int> adults;
    for (const auto& [name, age] : people) {
        if (age >= 18) {
            adults[name] = age;
        }
    }
    return adults;
}

bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        ++i;
    }
    return true;
}

set<int> TakePrimes(const set<int>& numbers) {
    set<int> primes;
    for (int number : numbers) {
        if (IsPrime(number)) {
            primes.insert(number);
        }
    }
    return primes;
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
    Document() : id(0),
                 relevance(0.0),
                 rating(0) {}

    Document(int id_, double relevance_, int rating_) : id(id_),
                                                        relevance(relevance_),
                                                        rating(rating_) {}

    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
   public:
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    explicit SearchServer(const string& stop_words) {
        SetStopWords(stop_words);
    }

    explicit SearchServer(const char* stop_words) {
        SetStopWords(static_cast<string>(stop_words));
    }

    template <typename StringCollection>
    explicit SearchServer(const StringCollection& collection_stop_words) {
        for (const auto& stop_word : collection_stop_words) {
            if (stop_word.size() != 0) {
                stop_words_.insert(stop_word);
            }
        }
    }

    [[nodiscard]] bool MatchDocument(const string& raw_query, int document_id,
                                     tuple<vector<string>, DocumentStatus>& result) const {
        Query query;
        vector<string> matched_documents;
        DocumentStatus status;

        if (!ParseQuery(raw_query, query)) {
            result = {matched_documents, status};
            return false;
        }

        if (documents_.count(document_id)) {
            status = documents_.at(document_id).status;
        } else {
            result = {matched_documents, status};
            return false;
        }

        if (!query.plus_words.empty()) {
            for (const auto& minus_word : query.minus_words) {
                if (word_to_document_freqs_.count(minus_word)) {
                    if (word_to_document_freqs_.at(minus_word).count(document_id)) {
                        result = {matched_documents, status};
                        return false;
                    }
                }
            }

            for (const auto& plus_word : query.plus_words) {
                if (word_to_document_freqs_.count(plus_word)) {
                    if (word_to_document_freqs_.at(plus_word).count(document_id)) {
                        matched_documents.push_back(plus_word);
                    }
                }
            }
        }
        result = {matched_documents, status};
        return true;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    [[nodiscard]] bool AddDocument(int document_id, const string& document,
                                   DocumentStatus status, const vector<int>& ratings) {
        if (document_id >= 0 && documents_.count(document_id) == 0) {
            vector<string> words;
            bool check = SplitIntoWordsNoStop(document, words);
            if (!check) {
                return false;
            }

            const double inv_word_count = 1.0 / words.size();
            for (const string& word : words) {
                word_to_document_freqs_[word][document_id] += inv_word_count;
            }
            documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
            document_id_direct_order_.push_back(document_id);
            return true;
        }
        return false;
    }

    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& raw_query,
                                        DocumentPredicate document_predicate,
                                        vector<Document>& result) const {
        Query query;
        vector<Document> matched_documents;

        if (!ParseQuery(raw_query, query) || query.plus_words.empty()) {
            result = matched_documents;
            return false;
        }

        matched_documents = FindAllDocuments(query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 double eps = 1e-6;
                 if (abs(lhs.relevance - rhs.relevance) < eps) {
                     return lhs.rating > rhs.rating;
                 }
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        result = matched_documents;
        return true;
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentStatus filter_status,
                                        vector<Document>& result) const {
        return FindTopDocuments(
            raw_query, [filter_status](int document_id, DocumentStatus status, int rating) {
                return status == filter_status;
            },
            result);
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, vector<Document>& result) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL, result);
    }

    int GetDocumentId(int index) const {  // index - порядковый номер
        if (index < 0 || index > documents_.size()) {
            return INVALID_DOCUMENT_ID;
        }
        return document_id_direct_order_[index];
    }

   private:
    static bool IsValidWord(const string& word) {
        int cnt_minus = 0;
        for (const char& ch : word) {
            if (ch == '-') {
                ++cnt_minus;
            } else {
                break;
            }
        }

        if (cnt_minus > 1 || word.size() == cnt_minus) {
            return false;
        }

        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    vector<int> document_id_direct_order_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    [[nodiscard]] bool SplitIntoWordsNoStop(const string& text, vector<string>& result) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsValidWord(word)) {
                result = words;
                return false;
            }

            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        result = words;
        return true;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = std::accumulate(ratings.begin(), ratings.end(), 0, [](const auto& l, const auto& r) {
            return l + r;
        });
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
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

    [[nodiscard]] bool ParseQuery(const string& text, Query& result) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsValidWord(word)) {
                result = query;
                return false;
            }

            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }

            } else {
                result = query;
                return false;
            }
        }

        result = query;
        return true;
    }

    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(documents_.size() * 1.0 / word_to_document_freqs_.at(word).size());
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
            matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

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
        ASSERT_EQUAL(server.FindTopDocuments("in"s, result), false);
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
        (void)server.AddDocument(3, "лиса потерялась"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
        vector<Document> result;
        bool check = server.FindTopDocuments("лиса -пропала и"s, result);
        ASSERT_EQUAL(check, false);
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
        SearchServer server("и в на");

        (void)server.AddDocument(1, "машина беспилотная"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(2, "автомобиль пропал"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});
        (void)server.AddDocument(3, "машина пропала белая"s, DocumentStatus::ACTUAL, {4, 4, 4, 4});

        tuple<vector<string>, DocumentStatus> result;
        bool res1 = server.MatchDocument("машина -беспилотная"s, 1, result);
        ASSERT_EQUAL(res1, false);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res2 = server.MatchDocument("машина -беспилотная"s, 2, result);
        ASSERT_EQUAL(res2, true);
        ASSERT_EQUAL(get<0>(result).size(), 0);

        bool res3 = server.MatchDocument("машина -беспилотная пропала"s, 3, result);
        ASSERT_EQUAL(res3, true);
        ASSERT_EQUAL(get<0>(result).size(), 2);

        bool res4 = server.MatchDocument("машина -беспилотная пропала"s, 4, result);
        ASSERT_EQUAL(res4, false);

        bool check = server.MatchDocument("и машина -беспилотная пропала"s, 3, result);
        ASSERT_EQUAL(check, false);
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

/*
void TestIsValidWord(){
    ASSERT_EQUAL(SearchServer::IsValidWord("--пушистый"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("------"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("-"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("\0"), false);
    ASSERT_EQUAL(SearchServer::IsValidWord("иван-чай"), true);
}
*/

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestAddDocument);
    RUN_TEST(TestSetStopWords);
    RUN_TEST(TestMatchDocument);
    RUN_TEST(TestSortRelevance);
    RUN_TEST(TestSplitIntoWords);
    RUN_TEST(TestGetDocumentId);

    //RUN_TEST(TestIsValidWord);
}

// --------- Окончание модульных тестов поисковой системы -----------

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
#ifdef _GLIBCXX_DEBUG
    TestSearchServer();
    cerr << "Search server testing finished"s << endl;
#endif

    SearchServer search_server("и в на"s);

    (void)search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});

    if (!search_server.AddDocument(1, "пушистый пёс модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }

    if (!search_server.AddDocument(-1, "пушистый пёс модный ошейник"s, DocumentStatus::ACTUAL, {1, 2})) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }

    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }

    (void)search_server.AddDocument(1, "и кот попался "s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
    (void)search_server.AddDocument(2, "и собака пропала"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});
    (void)search_server.AddDocument(3, "лиса потерялась"s, DocumentStatus::ACTUAL, {5, 3, 4, 1});

    vector<Document> documents;
    if (search_server.FindTopDocuments("лиса и"s, documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
}
