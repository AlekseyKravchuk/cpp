#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>  // for std::istringstream()
#include <string>
#include <utility>
#include <vector>

using namespace std;

// converts space-separated string "text" into vector of strings
vector<string> SplitIntoWords(const string& text) {
    istringstream iss(text);
    vector<string> words;
    string word;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}

int main() {
    string text = "some tasty oranges"s;
    return 0;
}
