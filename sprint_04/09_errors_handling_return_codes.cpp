#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

/*
Когда пишете надёжные программы, выдающие предсказуемые результаты, позаботьтесь об обработке нештатных ситуаций.
Для этих целей подойдут коды возврата и атрибут [[nodiscard]].
Он предупреждает вас о неиспользуемом результате вызова функции или метода.
*/

// Находим корни квадратного уравнения a*x^2 + b*x + c = 0
/*
pair<double, double> SolveQuadraticEquation(double a, double b, double c) {
    // Находим дискриминант
    double discriminant = b * b - 4 * a * c;

    // Находим корни уравнения
    double x1 = (-b - sqrt(discriminant)) / (2 * a);
    double x2 = (-b + sqrt(discriminant)) / (2 * a);

    return {x1, x2};
}
*/

// Improved version of SolveQuadraticEquation()
/* pair<double, double> SolveQuadraticEquation(double a, double b, double c, bool& success) {
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        success = false;
        // Функция должна вернуть пару значений. Пусть это будут нули
        return {0, 0};
    }
    success = true;

    double x1 = (-b - sqrt(discriminant)) / (2 * a);
    double x2 = (-b + sqrt(discriminant)) / (2 * a);
    return {x1, x2};
} */

// Improved version of SolveQuadraticEquation() V.2
[[nodiscard]] bool SolveQuadraticEquation(double a, double b, double c, pair<double, double>& roots) {
    double d = b * b - 4 * a * c;
    if (d < 0) {
        return false;
    }

    double x1 = (-b - sqrt(d)) / (2 * a);
    double x2 = (-b + sqrt(d)) / (2 * a);
    roots = {x1, x2};  // вычисленные значения корней положили в pair, переданный по ссылке
    return true;
}

int main() {
    /*  cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
    double a, b, c;
    cin >> a >> b >> c;
    const pair<double, double> roots = SolveQuadraticEquation(a, b, c);

    cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
    cout << "  x1="s << roots.first << "; x2="s << roots.second << endl; */

    /*     cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
    double a, b, c;
    cin >> a >> b >> c;

    bool success = false;
    const pair<double, double> roots = SolveQuadraticEquation(a, b, c, success);

    if (success) {
        cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
        cout << "  x1="s << roots.first << "; x2="s << roots.second << endl;
    } else {
        cout << "Уравнение не имеет действительных корней"s << endl;
    } */

    cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
    double a, b, c;
    cin >> a >> b >> c;
    pair<double, double> roots;

    if (SolveQuadraticEquation(a, b, c, roots)) {
        cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
        cout << "  x1="s << roots.first << "; x2="s << roots.second << endl;
    } else {
        cout << "Уравнение не имеет действительных корней"s << endl;
    }

    // Здесь программист забыл проверить результат вызова функции
    // SolveQuadraticEquation(a, b, c, roots);
    // cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
    // cout << "  x1="s << roots.first << "; x2="s << roots.second << endl;
}