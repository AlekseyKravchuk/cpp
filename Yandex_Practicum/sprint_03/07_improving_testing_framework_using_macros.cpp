#include <iostream>

using namespace std;

// #<имя параметра> подставляет строковое представление параметра макроса
#define PrintExpression(expr) cout << (#expr) << " = "s << (expr) << endl
#define Print(x) cout << (#x) << endl
#define LOG(expr) LogImpl(#expr, __FUNCTION__, __FILE__, __LINE__)
#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void LogImpl(const string& expression, const string& fileName, const string& functionName, int lineNumber) {
    cout << "file name: "s << fileName << endl;
    cout << "line: "s << lineNumber << endl;
    cout << "function name: "s << functionName << endl;
    cout << "expression: " << expression << endl;
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u,
                     const string& t_as_str, const string& u_as_str,
                     const string& fileName, const string& functionName,
                     const int lineNumber, const string& hint) {
    if (t != u) {
        cout << "file name: " << fileName << endl
             << "function name: " << functionName << endl
             << "ASSERT EQUAL("s << t_as_str << ", "s << u_as_str << ") failed: "s
             << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

int Sum(int a, int b) {
    // Ошибка допущена намеренно, чтобы продемонстрировать вывод информации об ошибке
    return a + b + 1;
}

int main() {
    // PrintExpression((8 + 15) * 3);
    // Функцию LogImpl можно вызывать напрямую, но это не очень удобно
    // LogImpl("12345"s, __FUNCTION__, __FILE__, __LINE__);
    // LogImpl("Hello, world!!!"s, __FUNCTION__, __FILE__, __LINE__);

    // LOG("Hello, world!!!"s);
    // LOG((12 + 3) * 100);

    // Print((12 + 18) * 10);

    string hello = "hello"s;
    ASSERT_EQUAL(hello.length(), 5);

    // Эта проверка не пройдёт
    ASSERT_EQUAL_HINT(Sum(2, 2), 4, "Sum() must be correct"s);

    return 0;
}
