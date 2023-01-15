#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

enum class Status {
    ACTUAL,
    EXPIRED,
    DELETED
};

struct Document {
    int id;
    Status status;
    double relevance;
    int rating;

    tuple<Status, int, double> MakeKey() const {
        // создаём и сразу возвращаем объект:
        // явно указывать его тип не нужно, поскольку
        // достаточно указать аргументы конструктора в фигурных скобках
        // но при этом тип возвращаемого методом значения должен быть указан ЯВНО (без "auto")
        return {status, -rating, -relevance};
    }
};

void SortDocuments(vector<Document>& matched_documents) {
    // Документы нужно упорядочить сначала по возрастанию статусов, а при их равенстве — по убыванию рейтинга, затем по убыванию релевантности.
    // Но при сравнении кортежей все компоненты сравниваются одинаково => при создании кортежа удобно умножить рейтинг и релевантность на −1.
    sort(matched_documents.begin(), matched_documents.end(),
         [](const Document& lhs, const Document& rhs) {
             return lhs.MakeKey() < rhs.MakeKey();
         });
}

int main() {
    vector<Document> documents = {
        {100, Status::ACTUAL, 0.5, 4},
        {101, Status::EXPIRED, 0.5, 4},
        {102, Status::ACTUAL, 1.2, 4},
        {103, Status::DELETED, 1.2, 4},
        {104, Status::ACTUAL, 0.3, 5},
    };

    SortDocuments(documents);

    for (const Document& document : documents) {
        cout << document.id << ' ' << static_cast<int>(document.status) << ' ' << document.relevance
             << ' ' << document.rating << endl;
    }

    return 0;
}