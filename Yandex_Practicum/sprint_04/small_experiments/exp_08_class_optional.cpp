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
    cout << "Введите коэффициенты уравнения a*x^2 + b*x + c = 0"s << endl;
    double a, b, c;
    cin >> a >> b >> c;
    // Вместо const optional<pair<double, double>> roots используем auto,
    //     позволяя компилятору вывести тип переменной roots самостоятельно
    if (const auto roots = SolveQuadraticEquation(a, b, c);
        roots.has_value()) {
        cout << "Корни уравнения "s << a << "*x^2 + "s << b << "*x + "s << c << " = 0"s << endl;
        // Доступ к значению можно получить при помощи метода roots.value() или (*roots)
        // Если внутри хранится структура или класс, то доступ к его полям
        //   можно получить при помощи ->
        cout << "  x1="s << roots.value().first << "; x2="s << roots->second << endl;
    } else {
        cout << "Уравнение не имеет действительных корней"s << endl;
    }
}
