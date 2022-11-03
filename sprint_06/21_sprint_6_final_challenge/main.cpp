#include <iostream>
#include <string>
#include <vector>

#include "document.h"
#include "paginator.h"
#include "read_input_functions.h"
#include "request_queue.h"
#include "search_server.h"
#include "string_processing.h"

// #define _DEBUG 1

// using namespace std::literals;
using namespace std;

void AddDocument(SearchServer& search_server,
                 int docID,
                 const std::string& rawDocument,
                 DocumentStatus status,
                 const std::vector<int>& ratings) {
    search_server.AddDocument(docID, rawDocument, status, ratings);
}

int main() {
    SearchServer search_server("and with"s);

    // #if defined _DEBUG
    // std::cout << "_DEBUG IS defined\n";
    // #else
    // std::cout << "_DEBUG IS NOT defined\n";
    // #endif  // _DEBUG

    // дубликат документа 2, будет удалён
    AddDocument(search_server, 22, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    AddDocument(search_server, 1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7, 2, 7});
    AddDocument(search_server, 2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // дубликат документа 2, будет удалён
    AddDocument(search_server, 3, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // дубликат документа 2, будет удалён
    AddDocument(search_server, 19, "funny pet with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // отличие только в стоп-словах, считаем дубликатом
    AddDocument(search_server, 4, "funny pet and curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // множество слов такое же, считаем дубликатом документа 1
    AddDocument(search_server, 5, "funny funny pet and nasty nasty rat"s, DocumentStatus::ACTUAL, {1, 2});

    // добавились новые слова, дубликатом не является
    AddDocument(search_server, 6, "funny pet and not very nasty rat"s, DocumentStatus::ACTUAL, {1, 2});

    // множество слов такое же, как в id 6, несмотря на другой порядок, считаем дубликатом
    AddDocument(search_server, 7, "very nasty rat and not very funny pet"s, DocumentStatus::ACTUAL, {1, 2});

    // есть не все слова, не является дубликатом
    AddDocument(search_server, 8, "pet with rat and rat and rat"s, DocumentStatus::ACTUAL, {1, 2});

    // слова из разных документов, не является дубликатом
    AddDocument(search_server, 9, "nasty rat with curly hair"s, DocumentStatus::ACTUAL, {1, 2});

    // cout << "Before duplicates removed: "s << search_server.GetDocumentCount() << endl;
    // RemoveDuplicates(search_server);
    // cout << "After duplicates removed: "s << search_server.GetDocumentCount() << endl;

    std::cout << "FINISHED"s << std::endl;
}
