// #include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>

using namespace std::literals;

/* 
На вход вашей программы в стандартном потоке ввода подаются действительные коэффициенты A, B и C
уравнения Ax² + Bx + C = 0.
Выведите все его различные действительные корни в поток вывода в любом порядке, при этом разделяя корни пробелами.
Гарантируется, что хотя бы один из коэффициентов уравнения не равен нулю..
 */

struct Result {
    double x1{};
    double x2{};
    bool hasAtLeastOneRoot = false;
};

Result solve(double a, double b, double c) {
    using std::sqrt;
    double x1{}, x2{};
    double D = b * b - 4 * a * c;

    // если "a == 0" - уравнение линейное bx + c = 0; x = -c/b;
    if (a == 0) {
        if (b != 0) {
            return {-c / b, -c / b, true};
        } else {
            return {{}, {}, false};
        }
    } else if (D == 0) {  // случай с нулевым дискриминантом => корень только один
        return {-b / (2 * a), -b / (2 * a), true};
    } else if (D > 0) {  // в случае с положительным дискриминантом корня два
        return {(-b + sqrt(D)) / (2 * a), (-b - sqrt(D)) / (2 * a), true};
    } else {  // если дискриминант отрицателен, действительных корней нет
        return {{}, {}, false};
    }
}

int main() {
    double a{}, b{}, c{};
    std::cin >> a >> b >> c;

    // C++17 "if statement" with initializer:
    // initialize the condition we want to check from within the if statement
    if (auto result = solve(a, b, c); result.hasAtLeastOneRoot) {
        if (result.x1 != result.x2) {
            std::cout << result.x1 << ' ' << result.x2 << std::endl;
        } else {
            std::cout << result.x1 << std::endl;
        }
    }

    return 0;
}
