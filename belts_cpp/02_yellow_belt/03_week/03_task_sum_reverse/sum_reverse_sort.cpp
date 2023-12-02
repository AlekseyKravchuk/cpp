#include "sum_reverse_sort.h"

#include <algorithm>

int Sum(int x, int y) {
    return x + y;
}

string Reverse(string s) {
    string s2;
    for (int i = static_cast<int>(s.size() - 1); i >= 0; --i) {
        s2.push_back(s[i]);
    }
    return s2;
}

void Sort(vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
}