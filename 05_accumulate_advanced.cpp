#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

// Функция FoldDistance превращает данные об очередном отрезке в суммарный путь
// Первый параметр - пройденное расстояние на данный момент
// Второй параметр - время и скорость движения на отрезке
// Возвращаемое значение - обновлённое расстояние
double FoldDistance(double current_distance, pair<double, double> section) {
    // Чтобы вычислить итоговый путь, нужно к текущему пути current_distance
    // прибавить длину отрезка section
    return current_distance + section.first * section.second;
}
int main() {
    // Информация о пройденных отрезках пути.
    // Каждая пара чисел содержит время (ч) и скорость движения на отрезке (км/ч)
    vector<pair<double, double>> sections = {{1.0, 5}, {0.5, 8}, {2, 3.5}};
    double total_distance = accumulate(sections.begin(), sections.end(), 5.0, FoldDistance);
    cout << "Total distance: "s << total_distance << " km"s << endl;
}
