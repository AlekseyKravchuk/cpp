#include <functional> // std::hash
#include <set>
#include <unordered_set>

#include "generator.h"
#include "print.h"
#include "profile.h"

// // set: 358 milliseconds
// // unordered_set: 1145 milliseconds
// // на выходе этой хэш-функци получается 3-значное число
// struct PlateHasher {
//     size_t operator()(const Plate& p) const {
//         return p.Number;
//     }
// };

// // set: 358 milliseconds
// // unordered_set: 54 milliseconds
// // на выходе этой хэш-функци получается 5-значное число
//
// // N = 5'000'000
// // set: 29690 milliseconds
// // unordered_set: 59168 milliseconds
// struct PlateHasher {
//     size_t operator()(const Plate& p) const {
//         return p.Number * 100 + p.Region;
//     }
// };

// // N = 5'000'000
// //set: 29849 milliseconds
// //unordered_set: 4372 milliseconds
struct PlateHasher {
    size_t operator()(const Plate& p) const {
        size_t result = p.Number;
        result *= 100;
        result += p.Region;
        result *= 1'000'000u;

        int m1 = p.C1 - 'A';
        int m2 = p.C2 - 'A';
        int m3 = p.C3 - 'A';
        int m = (m1 * 100 + m2) * 100 + m3;

        result += m;

        return result;
    }
};

// Напишем ещё одну собственную структуру, которая в качестве члена содержит другой пользовательский класс (структуру)
struct MyType {
    double d;
    std::string str;
    Plate plate;
};

struct MyHasher {
    size_t operator()(const MyType& obj) const {
        // вычисляем значения хешей для каждого из полей нашей структуры
        size_t h1 = double_hasher(obj.d);
        size_t h2 = str_hasher(obj.str);
        size_t h3 = plate_hasher(obj.plate);

        // теперь полученные хеши нужно объединить в одно общее значение
        // Универсальные совет - взять некоторый многочлен Ax^2 + Bx + C, где коэффициенты - это полученные значения хешей (от полей структуры),
        // а 'x' - некоторое простое число
        size_t x = 37;

        return (h1 * x * x + h2 * x + h3);
    }

    // используем стандартные хешеры из библиотеки (это функциональные объекты)
    std::hash<double> double_hasher;
    std::hash<std::string> str_hasher;
    PlateHasher plate_hasher;
};

int main() {
    std::unordered_set<Plate, PlateHasher> h_plates;
    std::unordered_set<MyType, MyHasher> h_my_types;
    // PlateGenerator pg;
    // std::set<Plate> s_plates;
    // std::unordered_set<Plate, PlateHasher> h_plates;

    return 0;
}
