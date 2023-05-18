#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> nonEmptyStrings;

    for (const string& str : strings) {
        if (!str.empty()) {
            nonEmptyStrings.insert(str);
        }
    }

    return nonEmptyStrings;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char ch : text) {
        if (ch == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += ch;
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

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
   public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stopWords) : _stopWords(MakeUniqueNonEmptyStrings(stopWords)) {
        // check stop words whether they contain special characters
        for (auto& stopWord : stopWords) {
            if (!IsValidWord(stopWord)) {
                string exception_message = "stop word: \""s + stopWord + "\" is not valid"s;
                throw invalid_argument(exception_message);
            }
        }
    }

    // Invoke delegating constructor from string container: SearchServer(SplitIntoWords(stop_words_text))
    explicit SearchServer(const string& stopWordsRawText)
        : SearchServer(SplitIntoWords(stopWordsRawText)) {}

    int GetDocumentCount() const { return _documents.size(); }

    // Document will be added in all cases except:
    // 1) docID < 0;
    // 2) document with given docID already exists in "_documents" data base
    // 3) if at least one word in the document contains special character
    void AddDocument(int docID,
                     const string& rawDocument,
                     DocumentStatus status,
                     const vector<int>& ratings) {
        if (docID < 0) {
            throw invalid_argument("passed document ID is negative"s);
        }

        bool documentAlreadyExists = _documents.find(docID) != _documents.end();
        if (documentAlreadyExists) {
            string exceptionMessage = "document with ID = "s + std::to_string(docID) + " already exists"s;
            throw invalid_argument(exceptionMessage);
        }

        vector<string> wordsNoStop = SplitIntoWordsNoStop(rawDocument);

        if (!std::all_of(wordsNoStop.begin(), wordsNoStop.end(), IsValidWord)) {
            string exceptionMessage = "document with ID = "s + std::to_string(docID) + " contains invalid characters"s;
            throw invalid_argument(exceptionMessage);
        }

        const double invertedWordCount = 1.0 / wordsNoStop.size();
        for (const string& word : wordsNoStop) {
            _word_docID_freqs[word][docID] += invertedWordCount;
        }

        _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});
        _numberingInOrder.push_back(docID);
    }

    // #1) FindTopDocuments, base function: TEMPLATE version of function with PREDICATE specified when calling from main
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& rawQuery,
                                      DocumentPredicate documentPredicate) const {
        vector<Document> foundDocs;
        Query parsedQuery = ParseQuery(rawQuery);

        if (parsedQuery.plusWords.empty()) {
            return foundDocs;
        }

        foundDocs = FindAllDocuments(parsedQuery, documentPredicate);
        if (!foundDocs.empty()) {
            sort(foundDocs.begin(), foundDocs.end(),
                 [](const Document& lhs, const Document& rhs) {
                     double reasonable_error = 1e-6;
                     if (abs(lhs.relevance - rhs.relevance) < reasonable_error) {
                         return lhs.rating > rhs.rating;
                     } else {
                         return lhs.relevance > rhs.relevance;
                     }
                 });
        }

        if (foundDocs.size() > MAX_RESULT_DOCUMENT_COUNT) {
            foundDocs.resize(MAX_RESULT_DOCUMENT_COUNT);
        }

        return foundDocs;
    }

    // #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
    vector<Document> FindTopDocuments(const string& rawQuery,
                                      DocumentStatus status) const {
        return FindTopDocuments(rawQuery,
                                [status](int document_id, DocumentStatus documentStatus, int rating) {
                                    return documentStatus == status;
                                });
    }

    // #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
    vector<Document> FindTopDocuments(const string& rawQuery) const {
        return FindTopDocuments(rawQuery, DocumentStatus::ACTUAL);
    }

    // Returns tuple: First component is a vector of PLUS-query-words found in document specified by docID
    //                Second component is STATUS of document specified by docID
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& rawQuery, int docID) const {
        vector<string> intersectionWithPlusWords;
        DocumentStatus status;

        if (_documents.count(docID)) {
            status = _documents.at(docID).status;
        } else {  // return empty tuple if document with "docID" isn't found
            return {intersectionWithPlusWords, status};
        }

        Query parsedQuery = ParseQuery(rawQuery);

        // return empty tuple if PLUS-words set of a query is empty
        if (parsedQuery.plusWords.empty()) {
            return {intersectionWithPlusWords, status};
        }

        // processing of MINUS-words: if any are found, return empty pair
        for (const string& minusWord : parsedQuery.minusWords) {
            // if "minusWord" is found in inverted index
            if (_word_docID_freqs.count(minusWord)) {
                // if "docID" key is found in map "docID : frequency"
                if (_word_docID_freqs.at(minusWord).count(docID)) {
                    return {intersectionWithPlusWords, status};
                }
            }
        }

        // processing of PLUS-words
        for (const string& plusWord : parsedQuery.plusWords) {
            if (_word_docID_freqs.count(plusWord)) {
                if (_word_docID_freqs.at(plusWord).count(docID)) {
                    if (!IsStopWord(plusWord)) {
                        intersectionWithPlusWords.push_back(plusWord);
                    }
                }
            }
        }

        return {intersectionWithPlusWords, status};
    }

    // return document identifier by its serial number
    int GetDocumentId(int serial_number) const {
        if (serial_number < 0 || serial_number > _documents.size()) {
            string exceptionMessage = __func__ + "(...): serial number of document is out of range"s;
            throw out_of_range(exceptionMessage);
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
        set<string> plusWords;
        set<string> minusWords;
    };

    set<string> _stopWords;
    map<string, map<int, double>> _word_docID_freqs;
    map<int, DocumentData> _documents;
    vector<int> _numberingInOrder;

    // returns true if NONE OF (НИ ОДИН ИЗ) the characters of the checked word does not belong to the range [\0; "SPACE")
    static bool IsWordWithoutSpecialChars(const string word2Check) {
        return none_of(word2Check.begin(),
                       word2Check.end(),
                       [](char ch) { return ch >= '\0' && ch < ' '; });
    }

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stopWords.insert(word);
        }
    }

    static bool IsValidWord(const string& word) {
        // check for multiple '-' in the beginning of a word and for a word consisting from only one '-'
        // minuses in the middle of a word are allowed, for example: "иван-чай", "-иван-чай"
        if (word[0] == '-') {
            if ((word.size() == 1) || (word[1] == '-')) {
                return false;
            }
        }

        // check for "-" at the end of a word
        if (word.back() == '-') {
            return false;
        }

        // final check for the absence of special characters
        return IsWordWithoutSpecialChars(word);
    }

    bool IsStopWord(const string& word) const {
        return _stopWords.count(word) > 0;
    }

    // returns vector of words composing the document content (excluding stop-words)
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

    QueryWord ParseQueryWord(string queryWord) const {
        if (queryWord.empty()) {
            string exceptionMessage = "query word: \""s + queryWord + "\" is empty"s;
            throw invalid_argument(exceptionMessage);
        }

        if (!IsValidWord(queryWord)) {
            string exception_message = "query word: \""s + queryWord + "\" is not valid"s;
            throw invalid_argument(exception_message);
        }

        bool isMinus = false;
        if (queryWord[0] == '-') {
            isMinus = true;
            queryWord = queryWord.substr(1);
        }
        return {queryWord, isMinus, IsStopWord(queryWord)};
    }

    Query ParseQuery(const string& rawQuery) const {
        Query parsedQuery;

        for (const string& word : SplitIntoWords(rawQuery)) {
            const QueryWord queryWord = ParseQueryWord(word);

            if (queryWord.is_minus) {
                parsedQuery.minusWords.insert(queryWord.data);
            } else {
                parsedQuery.plusWords.insert(queryWord.data);
            }
        }
        return parsedQuery;
    }

    // Existence required
    double ComputeInvertedDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / _word_docID_freqs.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate documentPredicate) const {
        map<int, double> doc2Relevance;

        for (const string& word : query.plusWords) {
            if (_word_docID_freqs.count(word) == 0) {
                continue;
            }

            const double inverseDocFrequency = ComputeInvertedDocumentFreq(word);
            for (const auto [docID, termFreq] : _word_docID_freqs.at(word)) {
                const auto& documentData = _documents.at(docID);
                if (documentPredicate(docID, documentData.status, documentData.rating)) {
                    doc2Relevance[docID] += termFreq * inverseDocFrequency;
                }
            }
        }

        for (const string& minusWord : query.minusWords) {
            // continue if given MINUS-word was NOT found among all the documents content
            if (_word_docID_freqs.count(minusWord) == 0) {
                continue;
            }

            // exclude from search result all the documents containing MINUS-words
            for (const auto [docID, _] : _word_docID_freqs.at(minusWord)) {
                doc2Relevance.erase(docID);
            }
        }

        vector<Document> foundDocs;
        for (const auto [docID, relevance] : doc2Relevance) {
            foundDocs.push_back(
                {docID, relevance, _documents.at(docID).rating});
        }
        return foundDocs;
    }
};

template <typename Iterator>
class IteratorRange {
   public:
    IteratorRange(Iterator pageBeginIt, Iterator pageEndIt) : _it_range(pageBeginIt, pageEndIt) {}

    Iterator begin() const {
        return _it_range.first;
    }

    Iterator end() const {
        return _it_range.second;
    }

    size_t size() const {
        return abs(distance(_it_range.first, _it_range.second));
    }

   private:
    pair<Iterator, Iterator> _it_range;
};

template <typename Iterator>
class Paginator {
   public:
    // constructor
    Paginator(Iterator beginIt, Iterator endIt, int pageSize) {
        auto pageStartIt = beginIt;
        auto pageEndIt = beginIt;
        int wholeNumOfPages = distance(beginIt, endIt) / pageSize;
        int remainder = distance(beginIt, endIt) % pageSize;

        if (wholeNumOfPages > 0) {
            // initialization of the end of page
            pageEndIt = next(pageStartIt, pageSize);

            for (int i = 0; i < wholeNumOfPages; ++i) {
                _pages.push_back(IteratorRange(pageStartIt, pageEndIt));
                pageStartIt = pageEndIt;
                pageEndIt = next(pageStartIt, pageSize);
            }
        }

        if (remainder > 0) {
            _pages.push_back(IteratorRange(pageStartIt, endIt));
        }
    }

    size_t size() const {
        return _pages.size();
    }

    auto begin() const {
        return _pages.begin();
    }

    auto end() const {
        return _pages.end();
    }

   private:
    vector<IteratorRange<Iterator>> _pages;
};

template <typename Container>
auto Paginate(const Container& container, size_t pageSize) {
    return Paginator(begin(container), end(container), pageSize);
}

ostream& operator<<(ostream& os, const Document& doc) {
    os << "{ document_id = "s << doc.id
       << ", relevance = " << doc.relevance
       << ", rating = " << doc.rating
       << " }";
    return os;
}

template <typename Iterator>
ostream& operator<<(ostream& os, const IteratorRange<Iterator>& range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        os << *it;
    }
    return os;
}

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

class RequestQueue {
   public:
    explicit RequestQueue(const SearchServer& searchServer) : _searchServerRef(searchServer) {}

    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& rawQuery, DocumentPredicate documentPredicate) {
        vector<Document> foundDocs = _searchServerRef.FindTopDocuments(rawQuery, documentPredicate);

        if (_requests.size() == _minutesInDay) {
            _requests.pop_front();
        }

        _requests.push_back({foundDocs, rawQuery, foundDocs.size() == 0});

        return foundDocs;
    }

    vector<Document> AddFindRequest(const string& rawQuery, DocumentStatus status) {
        return AddFindRequest(rawQuery,
                              [status](int document_id, DocumentStatus documentStatus, int rating) {
                                  return documentStatus == status;
                              });
    }

    vector<Document> AddFindRequest(const string& rawQuery) {
        return AddFindRequest(rawQuery, DocumentStatus::ACTUAL);
    }

    int GetNoResultRequests() const {
        return std::count_if(_requests.begin(),
                             _requests.end(),
                             [](const QueryResult& queryResult) {
                                 return queryResult.isEmptyResult;
                             });
    }

   private:
    struct QueryResult {
        vector<Document> foundDocs;
        const string rawQuery;
        bool isEmptyResult;
    };

    deque<QueryResult> _requests;
    const static int _minutesInDay = 1440;
    const SearchServer& _searchServerRef;
};

int main() {
    SearchServer search_server("and in at"s);
    RequestQueue requestQueue(search_server);

    search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, {1, 1, 1});

    // 1439 запросов с нулевым результатом
    for (int i = 0; i < 1439; ++i) {
        requestQueue.AddFindRequest("empty request"s);
    }

    // все еще 1439 запросов с нулевым результатом
    requestQueue.AddFindRequest("curly dog"s);

    // новые сутки, первый запрос удален, 1438 запросов с нулевым результатом
    requestQueue.AddFindRequest("big collar"s);

    // первый запрос удален, 1437 запросов с нулевым результатом
    requestQueue.AddFindRequest("sparrow"s);

    cout << "Total empty requests: "s << requestQueue.GetNoResultRequests() << endl;
    return 0;
}