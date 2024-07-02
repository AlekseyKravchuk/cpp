#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator beginIt, Iterator endIt) {
    for (auto it = beginIt; it != endIt; ++it) {
        if (std::next(it) != endIt) {
            std::cout << *it << " "s;
        } else {
            std::cout << *it << std::endl;
        }
    }
}

int main() {
    // Нужно найти в данном векторе ЯП, который начинается с буквы 'C'
    vector<string> langs = {"Python"s, "C++"s, "C"s, "Java"s, "C#"s};

    // std::find_if возвращает неконстантный итератор
    auto result = std::find_if(langs.begin(),
                            langs.end(),
                            [](const string& s) {
                                return s[0] == 'C';
                            });

    string& foundElementRef = *result;
    std::cout << "Found element: "s << foundElementRef << std::endl;

    *result = "D++"s;
    PrintRange(langs.begin(), langs.end());

    return 0;
}

// ============================= std :: find_if =============================
// Returns an iterator to the first element in the range [first, last) for which pred(Unary Function) returns true.
// If no such element is found, the function returns last.

// Function Template:
// InputIterator find_if (InputIterator first, InputIterator last, UnaryPredicate pred);

// first, last :range which contains all the elements between first
// and last, including the element pointed by first but
// not the element pointed by last.

// pred : Unary function that accepts an element in the range
// as argument and returns a value in boolean.

// Return value :
// Returns an iterator to the first element in the range
// [first, last) for which pred(function) returns true. If
// no such element is found, the function returns last.
// ============================================================================