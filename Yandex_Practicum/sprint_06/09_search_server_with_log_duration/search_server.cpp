#include "search_server.h"

int SearchServer::GetDocumentCount() const {
    return _documents.size();
}

vector<int> SearchServer::GetAllDocumentsIDs() const {
    std::vector<int> identifiers;

    for (const auto& [docID, _]: _documents) {
        identifiers.push_back(docID);
    }

    return identifiers;
}

// Document will be added in all cases except:
// 1) docID < 0;
// 2) document with given docID already exists in "_documents" data base
// 3) if at least one word in the document contains special character
void SearchServer::AddDocument(int docID,
                               const std::string& rawDocument,
                               DocumentStatus status,
                               const std::vector<int>& ratings) {
    if (docID < 0) {
        throw std::invalid_argument("passed document ID is negative"s);
    }

    bool documentAlreadyExists = _documents.find(docID) != _documents.end();
    if (documentAlreadyExists) {
        std::string exceptionMessage = "document with ID = "s + std::to_string(docID) + " already exists"s;
        throw std::invalid_argument(exceptionMessage);
    }

    std::vector<std::string> wordsNoStop = SplitIntoWordsNoStop(rawDocument);

    if (!std::all_of(wordsNoStop.begin(), wordsNoStop.end(), IsValidWord)) {
        std::string exceptionMessage = "document with ID = "s + std::to_string(docID) + " contains invalid characters"s;
        throw std::invalid_argument(exceptionMessage);
    }

    const double invertedWordCount = 1.0 / wordsNoStop.size();
    for (const std::string& word : wordsNoStop) {
        _word_docID_freqs[word][docID] += invertedWordCount;
    }

    _documents.emplace(docID, DocumentData{ComputeAverageRating(ratings), status});
    _numberingInOrder.push_back(docID);
}

// #2) FindTopDocuments, WRAPPER: converts STATUS-based to Predicate-based logic
std::vector<Document> SearchServer::FindTopDocuments(const std::string& rawQuery,
                                                     DocumentStatus status) const {
    return SearchServer::FindTopDocuments(rawQuery,
                                          [status](int document_id, DocumentStatus documentStatus, int rating) {
                                              return documentStatus == status;
                                          });
}

// #3) FindTopDocuments, WRAPPER: check for ACTUAL status by default
std::vector<Document> SearchServer::FindTopDocuments(const std::string& rawQuery) const {
    return SearchServer::FindTopDocuments(rawQuery, DocumentStatus::ACTUAL);
}

// Returns tuple: First component is a vector of PLUS-query-words found in document specified by docID
//                Second component is STATUS of document specified by docID
std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& rawQuery, int docID) const {
    std::vector<std::string> intersectionWithPlusWords;
    DocumentStatus status;

    if (_documents.count(docID)) {
        status = _documents.at(docID).status;
    } else {  // return empty tuple if document with "docID" isn't found
        return {intersectionWithPlusWords, status};
    }

    Query parsedQuery = SearchServer::ParseQuery(rawQuery);

    // return empty tuple if PLUS-words set of a query is empty
    if (parsedQuery.plusWords.empty()) {
        return {intersectionWithPlusWords, status};
    }

    // processing of MINUS-words: if any are found, return empty pair
    for (const std::string& minusWord : parsedQuery.minusWords) {
        // if "minusWord" is found in inverted index
        if (_word_docID_freqs.count(minusWord)) {
            // if "docID" key is found in map "docID : frequency"
            if (_word_docID_freqs.at(minusWord).count(docID)) {
                return {intersectionWithPlusWords, status};
            }
        }
    }

    // processing of PLUS-words
    for (const std::string& plusWord : parsedQuery.plusWords) {
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
int SearchServer::GetDocumentId(int serial_number) const {
    if (serial_number < 0 || serial_number > static_cast<int>(_documents.size())) {
        std::string exceptionMessage = __func__ + "(...): serial number of document is out of range"s;
        throw std::out_of_range(exceptionMessage);
    } else {
        return _numberingInOrder[serial_number];
    }
}

// returns true if NONE OF (НИ ОДИН ИЗ) the characters of the checked word does not belong to the range [\0; "SPACE")
bool SearchServer::IsWordWithoutSpecialChars(const std::string word2Check) {
    return std::none_of(word2Check.begin(),
                        word2Check.end(),
                        [](char ch) { return ch >= '\0' && ch < ' '; });
}

void SearchServer::SetStopWords(const std::string& text) {
    for (const std::string& word : SplitIntoWords(text)) {
        _stopWords.insert(word);
    }
}

bool SearchServer::IsValidWord(const std::string& word) {
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

bool SearchServer::IsStopWord(const std::string& word) const {
    return _stopWords.count(word) > 0;
}

// returns vector of words composing the document content (excluding stop-words)
std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {
    std::vector<std::string> words;

    for (const std::string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }

    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
    return (ratings.empty()) ? 0 : std::reduce(ratings.begin(), ratings.end(), 0.0) / static_cast<double>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string queryWord) const {
    if (queryWord.empty()) {
        std::string exceptionMessage = "query word: \""s + queryWord + "\" is empty"s;
        throw std::invalid_argument(exceptionMessage);
    }

    if (!SearchServer::IsValidWord(queryWord)) {
        std::string exception_message = "query word: \""s + queryWord + "\" is not valid"s;
        throw std::invalid_argument(exception_message);
    }

    bool isMinus = false;
    if (queryWord[0] == '-') {
        isMinus = true;
        queryWord = queryWord.substr(1);
    }
    return {queryWord, isMinus, SearchServer::IsStopWord(queryWord)};
}

SearchServer::Query SearchServer::ParseQuery(const std::string& rawQuery) const {
    SearchServer::Query parsedQuery;

    for (const std::string& word : SplitIntoWords(rawQuery)) {
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
double SearchServer::ComputeInvertedDocumentFreq(const std::string& word) const {
    return log(SearchServer::GetDocumentCount() * 1.0 / _word_docID_freqs.at(word).size());
}

