#include "request_queue.h"

std::vector<Document> RequestQueue::AddFindRequest(const std::string& rawQuery, DocumentStatus status) {
    return AddFindRequest(rawQuery,
                          [status](int document_id, DocumentStatus documentStatus, int rating) {
                              return documentStatus == status;
                          });
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& rawQuery) {
    return AddFindRequest(rawQuery, DocumentStatus::ACTUAL);
}

int RequestQueue::GetNoResultRequests() const {
    return std::count_if(_requests.begin(),
                         _requests.end(),
                         [](const QueryResult& queryResult) {
                             return queryResult.isEmptyResult;
                         });
}
