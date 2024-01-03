#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Spending {
    std::string category;
    int amount;
};

bool operator==(const Spending& lhs, const Spending& rhs);

std::ostream& operator<<(std::ostream& os, const Spending& s);

int CalculateTotalSpendins(const std::vector<Spending>& spendings);

std::string MostExpensiveCategory(const std::vector<Spending>& spendings);

std::vector<Spending> LoadFromXml(std::istream& input);
