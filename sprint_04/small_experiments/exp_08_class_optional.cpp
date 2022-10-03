#include <cmath>
#include <iostream>
#include <optional>
#include <utility>

using namespace std;

// Функция возвращает пару корней квадратного уравнения либо пустое значение, когда решения нет
optional<pair<double, double>> SolveQuadraticEquation(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        // Возвращаем специальное значение nullopt, означающее отсутствие значения
        return nullopt;
    }

    double x1 = (-b - sqrt(discriminant)) / (2 * a);
    double x2 = (-b + sqrt(discriminant)) / (2 * a);

    // Возвращаем корни уравнения
    return pair{x1, x2};
}

int main() {
    return 0;
}
