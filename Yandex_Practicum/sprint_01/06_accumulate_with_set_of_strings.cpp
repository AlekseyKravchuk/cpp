#include <numeric>
#include <set>
#include <iostream>
#include <string>

using namespace std;

string makeTail(const string& s1, const string& s2) {
    return s1 + " -"s + s2;
}

string AddStopWords(const string& query, const set<string>& stop_words) {
    return accumulate(stop_words.begin(), stop_words.end(), query, makeTail);
}

int main() {
    string query = "some tasty oranges"s;
    set<string> stop_words = {"of"s, "in"s};
    string result = AddStopWords(query, stop_words);
    cout << "result: " << result << endl;

    return 0;
}
