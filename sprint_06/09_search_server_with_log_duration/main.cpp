#include <iostream>
#include <string>
#include <vector>

#include "document.h"
#include "log_duration.h"
#include "paginator.h"
#include "read_input_functions.h"
#include "request_queue.h"
#include "search_server.h"
#include "string_processing.h"

using namespace std::literals;
// using namespace std;

template <typename T>
std::ostream& std::operator<<(std::ostream& os, const std::vector<T>& val) {
    bool isFirstStr = true;
    for (auto str : val) {
        if (isFirstStr) {
            os << str;
            isFirstStr = false;
        } else {
            os << ", " << str;
        }
    }

    return os;
}

void MatchDocuments(const SearchServer& searchServer, const std::string rawQuery) {
    std::cout << "Матчинг документов по запросу: "s << rawQuery << std::endl;
    for (auto docID : searchServer.GetAllDocumentsIDs()) {
        auto [intersectionWithPlusWords, status] = searchServer.MatchDocument(rawQuery, docID);
        std::cout << "{ document_id = "s << docID
                  << ", status = "s << static_cast<int>(status)
                  << ", words = "s << intersectionWithPlusWords
                  << "}"
                  << std::endl;
    }
}

void FindTopDocuments(const SearchServer& searchServer, const std::string rawQuery) {
    std::cout << "Результаты поиска по запросу: "s << rawQuery << std::endl;
    for (auto document : searchServer.FindTopDocuments(rawQuery)) {
        std::cout << document << std::endl;
    }
}

int main() {
    SearchServer search_server("and in at"s);
    RequestQueue requestQueue(search_server);

    search_server.AddDocument(6, "curly cat curly tail"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(8, "big cat fancy collar "s, DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(23, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, {1, 1, 1});

    // code from Sprint 5:
    /* // 1439 запросов с нулевым результатом
    for (int i = 0; i < 1439; ++i) {
        requestQueue.AddFindRequest("empty request"s);
    }

    // все еще 1439 запросов с нулевым результатом
    requestQueue.AddFindRequest("curly dog"s);

    // новые сутки, первый запрос удален, 1438 запросов с нулевым результатом
    requestQueue.AddFindRequest("big collar"s);

    // первый запрос удален, 1437 запросов с нулевым результатом
    requestQueue.AddFindRequest("sparrow"s);

    std::cout << "Total empty requests: "s << requestQueue.GetNoResultRequests() << std::endl; */

    // без макроса
    {
        LogDuration guard("Long task", cout);
        MatchDocuments(search_server, "collar cat -dog"s);
    }

    std::cout << "==============================================="s << endl;
    // с макросом
    {
        LOG_DURATION_STREAM("Long task", cout);
        MatchDocuments(search_server, "collar cat -dog"s);
    }
    std::cout << "==============================================="s << endl;
    // без макроса
    {
        LogDuration guard("Long task", cout);
        FindTopDocuments(search_server, "collar cat -dog"s);
    }
    std::cout << "==============================================="s << endl;
    // с макросом
    {
        LOG_DURATION_STREAM("Long task", cout);
        FindTopDocuments(search_server, "collar cat -dog"s);
    }
    std::cout << "==============================================="s << endl;

    return 0;
}
