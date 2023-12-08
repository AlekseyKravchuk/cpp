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
        size_t first = static_cast<size_t>((p.Number * 100 + p.Region)) * 1'000'000u;
        size_t second = (((p.C1 - 'A') * 100) + (p.C2 - 'A')) * 100 + (p.C3 - 'A');
        size_t result = first + second;
        return result;
    }
};

struct MyType {
    double d;
    std::string str;
    Plate plate;
};

struct MyHasher {
    size_t operator()(const MyType& obj) const {
        size_t h1 = double_hasher(obj.d);
        size_t h2 = str_hasher(obj.str);
        size_t h3 = plate_hasher(obj.plate);

        // Ax^2 + Bx + C
        // В литературе принято брать в качестве значения 'x' какое-нибудь простое число
        size_t x = 37;

        return (h1 * x * x + h2 * x + h3);
    }

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
