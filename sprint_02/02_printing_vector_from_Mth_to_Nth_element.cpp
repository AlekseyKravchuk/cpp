#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    int rating;
};

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
    if ( documents.empty() || documents.size() <= (skip_start + skip_finish) ) {
        return;
    }

    // отсортировать документы и вывести не все
    sort(documents.begin(), documents.end(),
         [](const Document& lhs, const Document& rhs) {
             if (lhs.rating != rhs.rating) {
                 return lhs.rating > rhs.rating;
             } else {
                 return lhs.id > rhs.id;
             }
         });

    auto it = documents.begin() + skip_start;
    auto it_finish = documents.rbegin() + skip_finish;
    for (; it != it_finish.base(); ++it) {
        cout << "{ id = "s << (*it).id << ", rating = "s << (*it).rating << " }"s << endl;
    }
}

int main() {
    PrintDocuments({{100, 5}, {101, 7}, {102, -4}, {103, 9}, {104, 1}}, 3, 4);
}