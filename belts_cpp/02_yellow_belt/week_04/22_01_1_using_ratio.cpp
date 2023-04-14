#include <cstdint>  // to work with FIXED WIDTH integer types (since C++11)
#include <iostream>
#include <limits>
#include <ratio>
#include <string>

using namespace std::literals;

int main() {
    // Числитель (Numerator) и знаменатель (Denominator) рационального числа представлены
    // как константы ЭТАПА КОМПИЛЯЦИИ типа "std::intmax_t" (определена в "cstdint")

    // Ввиду характера времени компиляции, объекты типа "ratio" нельзя создавать, как обычные объекты и вызывать методы посредством этих объектов.
    // Вместо этого используют псевдонимы типов (type aliases).

    using r1 = std::ratio<1, 60>;

    // Числитель "numerator" и знаменатель "denominator" рационального числа "r1"
    // являются КОНСТАНТАМИ ЭТАПА КОМПИЛЯЦИИ (compile-time constants), т.е. их значения должны быть известны на этапе компиляции.
    // Получить доступ к ним можно следующим образом:
    intmax_t numerator{r1::num};
    intmax_t denominator{r1::den};
    std::cout << "numerator: " << numerator << std::endl;
    std::cout << "denominator: " << denominator << std::endl;

    // // тип "intmax_t" по сути является псевдонимом "long long"
    // std::cout << std::numeric_limits<intmax_t>::max() << std::endl;
    // std::cout << std::numeric_limits<long long>::max() << std::endl;

    // std::cout << std::endl;

    // // тип "uintmax_t" по сути является псевдонимом "unsigned long long"
    // std::cout << std::numeric_limits<uintmax_t>::max() << std::endl;
    // std::cout << std::numeric_limits<unsigned long long>::max() << std::endl;

    // // следующий код генерирует ошибку компиляции
    // intmax_t n{1};                // Numerator
    // intmax_t d{60};               // Denominator
    // using r1 = std::ratio<n, d>;  // Error

    // Если объявить "n" и "d" константами, код скомпилируется
    const intmax_t n{1};          // Numerator
    const intmax_t d{60};         // Denominator
    using r1 = std::ratio<n, d>;  // OK

    // Библиотека "Ratio" поддерживает сложение, вычитание, умножение и деление рациональных чисел.
    // НО! Поскольку эти операции выполняются на этапе компиляции, их можно использовать только со специальными шаблонами в комбинации с псевдонимами типов.
    // Доступны следующие шаблоны: "ratio_add", "ratio_subtract", "ratio_multiply", и "ratio_divide".

    // Эти шаблоны вычисляют результат как новый тип "ratio".
    // Например, следующий код сначала определяет два числа типа "ration", одно из которых представляет 1/60, а другое — 1/30.
    // Шаблон "ratio_add" складывает эти два рациональных числа вместе, чтобы получить результирующее рациональное число,
    // которое после нормализации равно 1/20:
    using r1 = std::ratio<1, 60>;
    using r2 = std::ratio<1, 30>;
    using result = std::ratio_add<r1, r2>::type;
    intmax_t n_res{result::num};
    intmax_t d_res{result::den};
    std::cout << std::endl;
    std::cout << "n_res: " << n_res << std::endl;
    std::cout << "d_res: " << d_res << std::endl;

    return 0;
}
