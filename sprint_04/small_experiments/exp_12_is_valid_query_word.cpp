#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// returns true if NONE OF (НИ ОДИН ИЗ) the characters of the checked word does not belong to the range [\0; "SPACE")
bool IsWordWithoutSpecialChars(const string word2Check) {
    return none_of(word2Check.begin(),
                   word2Check.end(),
                   [](char ch) { return ch >= '\0' && ch < ' '; });
}

static bool IsValidQueryWord(const string& queryWord) {
    size_t minusCounter = 0;

    // check for multiple '-' in the beginning of a word and for a word consisting from only one '-'
    // minuses in the middle of a word are allowed, for example: "иван-чай", "-иван-чай"
    if (queryWord[0] == '-') {
        if ( (queryWord.size() == 1) || (queryWord[1] == '-') ) {
            return false;
        }
    }

    // check for "-" at the end of a word
    if (queryWord.back() == '-') {
        return false;
    }

    // final check for the absence of special characters
    return IsWordWithoutSpecialChars(queryWord);
}

int main() {
    const string string2check = "-иван-чай"s;
    cout << boolalpha << IsValidQueryWord(string2check) << endl;

    return 0;
}
