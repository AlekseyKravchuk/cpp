#include <string>

using namespace std::literals;

template <typename T, typename U>
struct S {  // конструктора НЕТ!
    T _first;
    U _second;
};

// Мой deduction guide (пользовательская подсказка для выведения типа компилятором)
template <typename T, typename U>
S(const T& first, const U& second) -> S<T, U>;

template <typename T, typename U>
S(const T& first, const char* second) -> S<T, std::string>;

// также можно указать компилятору, чтобы он выводил типы по значению
// template <typename T, typename U>
// S(const T first, const U second) -> S<T, U>;

int main() {

    S s = {1, "hello"};

    return 0;
}
