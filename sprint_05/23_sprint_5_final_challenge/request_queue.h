#pragma once

//#ifndef __REQUEST_QUEUE__H__
//#define __REQUEST_QUEUE__H__

#include <algorithm>
#include <deque>
#include <string>
#include <vector>

#include "search_server.h"
#include "document.h"

class RequestQueue {
   public:
    explicit RequestQueue(const SearchServer& searchServer) : _searchServerRef(searchServer) {}

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& rawQuery, DocumentPredicate documentPredicate);

    std::vector<Document> AddFindRequest(const std::string& rawQuery, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& rawQuery);
    int GetNoResultRequests() const;

   private:
    struct QueryResult {
        std::vector<Document> foundDocs;
        const std::string rawQuery;
        bool isEmptyResult;
    };

    std::deque<QueryResult> _requests;
    const static int _minutesInDay = 1440;
    const SearchServer& _searchServerRef;
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& rawQuery, DocumentPredicate documentPredicate) {
    std::vector<Document> foundDocs = _searchServerRef.FindTopDocuments(rawQuery, documentPredicate);

    if (_requests.size() == _minutesInDay) {
        _requests.pop_front();
    }

    _requests.push_back({foundDocs, rawQuery, foundDocs.size() == 0});

    return foundDocs;
}

//#endif  //!__REQUEST_QUEUE__H__
