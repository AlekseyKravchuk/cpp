#include "spending.h"

using namespace std;

bool operator==(const Spending& lhs, const Spending& rhs) {
    return lhs.category == rhs.category && lhs.amount == rhs.amount;
}

ostream& operator<<(ostream& os, const Spending& s) {
    return os << '(' << s.category << ": " << s.amount << ')';
}

// вычисляет, сколько всего денег было потрачено по вектору "spendings"
int CalculateTotalSpendins(const vector<Spending>& spendings) {
    int result = 0;

    for (const Spending& s : spendings) {
        result += s.amount;
    }

    return result;
}

string MostExpensiveCategory(const vector<Spending>& spendings) {
    return max_element(spendings.begin(),
                       spendings.end(),
                       [](const Spending& lhs, const Spending& rhs) {
                           return lhs.amount < rhs.amount;
                       })
        ->category;
}
