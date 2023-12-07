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


int main() {
    PlateGenerator pg;
    std::set<Plate> s_plates;
    std::unordered_set<Plate, PlateHasher> h_plates;
    const int N = 5'000'000;

    // Plate p = {'M', 463, 'C', 'A', 24};
    // h_plates.insert(p);

    {
        LOG_DURATION("set");
        for (int i = 0; i < N; ++i) {
            s_plates.insert(pg.GetRandomPlate());
        }

        for (int i = 0; i < N; ++i) {
            s_plates.find(pg.GetRandomPlate());
        }
    }

    {
        LOG_DURATION("unordered_set");
        for (int i = 0; i < N; ++i) {
            h_plates.insert(pg.GetRandomPlate());
        }

        for (int i = 0; i < N; ++i) {
            h_plates.find(pg.GetRandomPlate());
        }
    }

    // std::cout << std::left << std::setw(18) << "set: " << s_plates << std::endl;

    // for (int i = 0; i < N; ++i) {
    //     h_plates.insert(pg.GetRandomPlate());
    // }
    // std::cout << std::left << std::setw(18) << "unordered_set: " << h_plates << std::endl;

    return 0;
}
