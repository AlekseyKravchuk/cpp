#include <algorithm>
#include <cctype>  // for std::isspace(int ch)
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Определяет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные символы английского алфавита и пробелы
// Пустые строки и строки, состоящие только из пробелов, — это не палиндромы
bool IsPalindrome(const string& text) {
    // there are two isspace() functions, the one from <cctype> is what we need here,
    // while the other from <locale> is not. The way to resolve it is to use ::isspace as the predicate
    if (text.empty() || all_of(text.begin(), text.end(), ::isspace)) {
        return false;
    }

    // Second way to check if string contains only whitespaces
    /*     if (text.empty() || text.find_first_not_of(' ') == std::string::npos) {
        return false;
    } */

    istringstream iss(text);
    string word;
    string concatenated;

    while (iss >> word) {
        concatenated += word;
    }

    if (concatenated == string(concatenated.rbegin(), concatenated.rend())) {
        return true;
    } else {
        return false;
    }
}

int main() {
    string text;
    getline(cin, text);

    if (IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    } else {
        cout << "not a palindrome"s << endl;
    }
}