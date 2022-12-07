#include <algorithm>
#include <utility>
#include <vector>

#include "../log_duration.h"

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

// Так выглядит обычная сортировка документов:
void SortDocuments(vector<Document>& documents) {
    LOG_DURATION("default");
    sort(documents.begin(), documents.end(),
         [](const Document& lhs, const Document& rhs) {
             return pair(lhs.rating, lhs.relevance) > pair(rhs.rating, rhs.relevance);
         });
}

int main() {
    /* code */
    return 0;
}
