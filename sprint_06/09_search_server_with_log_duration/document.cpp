#include "document.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std::literals;

void PrintDocument(const Document& document) {
    std::cout << "{ "s
              << "document_id = "s << document.id << ", "s
              << "relevance = "s << document.relevance << ", "s
              << "rating = "s << document.rating << " }"s << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Document& doc) {
    os << "{ document_id = "s << doc.id
       << ", relevance = " << doc.relevance
       << ", rating = " << doc.rating
       << " }";
    return os;
}
