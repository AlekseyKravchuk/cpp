#pragma once
//#ifndef __DOCUMENT__H__
//#define __DOCUMENT__H__

#include <fstream>

#include "paginator.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id), relevance(relevance), rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

void PrintDocument(const Document& document);
std::ostream& operator<<(std::ostream& os, const Document& doc);

//#endif  //!__DOCUMENT__H__
