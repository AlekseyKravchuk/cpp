#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

#define ASSERT(expr) AssertImpl((expr), (#expr), __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl((expr), (#expr), __FILE__, __FUNCTION__, __LINE__, hint)

void AssertImpl(bool expr, const string& exprAsStr,
                const string& fileName, const string& functionName,
                const int lineNumber, const string& hint) {
    if (!expr) {
        cout << fileName << "("s << lineNumber << "): "s
             << functionName << ": "s
             << "ASSERT("s << exprAsStr << ") failed."s;

        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

int main() {
    string hello = "hello"s;
    // ASSERT(hello.empty());

    ASSERT_HINT(2 + 2 == 5, "This will fail"s);

    return 0;
}