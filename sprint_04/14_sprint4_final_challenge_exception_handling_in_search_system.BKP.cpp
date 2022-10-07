#include <algorithm>
#include <cmath>
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

// returns true if NONE OF (НИ ОДИН ИЗ) the characters of the checked word does not belong to the range [\0; "SPACE")
bool IsWordWithoutSpecialChars(const string word2Check) {
    return none_of(word2Check.begin(),
                   word2Check.end(),
                   [](char ch) { return ch >= '\0' && ch < ' '; });
}

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> nonEmptyStrings;
    for (const string& str : strings) {
        if (!str.empty()) {
            if (IsWordWithoutSpecialChars(str)) {
                nonEmptyStrings.insert(str);
            } else {
                string exceptionMessage =
                    "some words passed to  MakeUniqueNonEmptyStrings(...) contain invalid characters,\
                                           i.e., characters with codes from 0 to 31"s;
                throw invalid_argument(exceptionMessage);
            }
        }
    }
    return nonEmptyStrings;
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
        if (IsWordWithoutSpecialChars(word)) {
            words.push_back(word);
        } else {
            string part1 = "some of the words passed to the function \""s;
            string part2 = __func__;
            string part3 = "(...)\" contain invalid characters,i.e. characters with codes from 0 to 31"s;
            string exceptionMessage = part1 + part2 + part3;

            throw invalid_argument(exceptionMessage);
        }
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
    explicit SearchServer(const StringContainer& stop_words)
        : _stopWords(MakeUniqueNonEmptyStrings(stop_words)) {}

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
        bool documentAlreadyExists = _documents.find(docID) != _documents.end();

        if (docID < 0) {
            throw invalid_argument("passed document ID is negative"s);
        }

        if (documentAlreadyExists) {
            string exceptionMessage = "document with ID = "s + std::to_string(docID) + " already exists"s;
            throw invalid_argument(exceptionMessage);
        }

        vector<string> wordsNoStop = SplitIntoWordsNoStop(rawDocument);

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
                                [status](int document_id, DocumentStatus document_status, int rating) {
                                    return document_status == status;
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

    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            _stopWords.insert(word);
        }
    }

    static bool IsValidQueryWord(const string& queryWord) {
        size_t minusCounter = 0;

        // check for multiple '-' in the beginning of a word and for a word consisting from only one '-'
        // minuses in the middle of a word are allowed, for example: "иван-чай", "-иван-чай"
        if (queryWord[0] == '-') {
            if ((queryWord.size() == 1) || (queryWord[1] == '-')) {
                return false;
            }
        }

        // check for "-" at the end of a word
        if (queryWord.back() == '-') {
            return false;
        }

        // final check for the absence of special characters
        return IsWordWithoutSpecialChars(queryWord);
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

        if (!IsValidQueryWord(queryWord)) {
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

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    SearchServer searchServer("и в на"s);

    searchServer.AddDocument(0, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    searchServer.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
    searchServer.AddDocument(2, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
    searchServer.AddDocument(5, "большой пёс скворец"s, DocumentStatus::ACTUAL, {1, 3, 2});

    // const auto foundDocs = searchServer.FindTopDocuments("пушистый -пёс"s);

    // ================== Driver code resulting in throwing exceptions: ==================
    searchServer.GetDocumentId(188);
    searchServer.AddDocument(12, "большой пёс ск\bворец"s, DocumentStatus::ACTUAL, {1, 3, 2});
    SearchServer searchServer2("и в н\tа"s);
    searchServer.AddDocument(-5, "большой пёс скворец"s, DocumentStatus::ACTUAL, {1, 3, 2});
    searchServer.AddDocument(5, "большой пёс скворец"s, DocumentStatus::ACTUAL, {1, 3, 2});
    const auto foundDocs = searchServer.FindTopDocuments("пушистый -п\tёс"s);
    // =============== End of driver code resulting in throwing exceptions: ==============

    return 0;
}