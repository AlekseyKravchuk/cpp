#include <algorithm>

// Писать качественный и эффективный параллельный код непросто.
// Но авторы алгоритмов стандартной библиотеки C++ сделали это за вас!
// Достаточно подключить библиотеку "<execution>" и передать "std::execution::par"
// в качестве первого аргумента в нужный алгоритм.
#include <execution>
#include <utility>
#include <vector>

#include "log_duration.h"

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

void SortDocuments(vector<Document>& documents) {
    LOG_DURATION("parallel");
    sort(execution::par,  // !!!
         documents.begin(), documents.end(),
         [](const Document& lhs, const Document& rhs) {
             return pair(lhs.rating, lhs.relevance) > pair(rhs.rating, rhs.relevance);
         });
}

int main() {
    
    return 0;
}
