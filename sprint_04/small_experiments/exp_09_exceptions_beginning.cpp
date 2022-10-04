#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace std;

pair<double, double> SolveQuadraticEquation(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        throw domain_error("квадратное уравнение не имеет действительных корней"s);
    }

    double x1 = (-b - sqrt(discriminant)) / (2 * a);
    double x2 = (-b + sqrt(discriminant)) / (2 * a);
    return {x1, x2};
}

int main() {
    try {
        cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
        double a, b, c;
        cin >> a >> b >> c;

        pair<double, double> roots = SolveQuadraticEquation(a, b, c);
        cout << "Корни: уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
        cout << "  x1="s << roots.first << "; x2="s << roots.second << endl;
    } catch (const domain_error& error) {
        // При помощи метода what() можно получить строковое представление текста ошибки
        cout << "Ошибка: "s << error.what() << endl;
    }
}