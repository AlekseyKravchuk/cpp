#include <algorithm>  // for usage of lexicographical_compare()
#include <iostream>
#include <string>
#include <vector>
// #include <sstream>

using namespace std;

bool CompIgnoreCase(char ch1, char ch2) {
    return tolower(ch1) < tolower(ch2);
}

// A function object to do lexicographical comparisons
template <typename Container>
bool LexicographicCompare(const Container& a, const Container& b) {
    return std::lexicographical_compare(a.begin(), a.end(),
                                        b.begin(), b.end(),
                                        CompIgnoreCase);
}

// Use that comparison function to sort a range:
template <typename ContainerIterator>
void sort_by_lexicographical_comapre(ContainerIterator beg,
                                     ContainerIterator end) {
    std::sort(beg, end, LexicographicCompare<typename ContainerIterator::value_type>);
}

int main() {
    // int n = 8;
    // vector<string> v = {"quantity"s, "AMOUNT"s, "Selection"s, "Search"s, "4ever"s, "music"s, "Mickey"s, "market"};

    int n;
    string word;
    vector<string> v;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> word;
        v.push_back(word);
    }

    // второй вариант лексикографической сортировки
    sort_by_lexicographical_comapre(v.begin(), v.end());

    for (const auto& word : v) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}
