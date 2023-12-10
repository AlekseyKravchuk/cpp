#pragma once

#include <cstdlib>  // size_t

#include "plate.h"

// // самый простой hasher - просто возвращаем центральное число автомобильного номера (plate)
// // на выходе этой хэш-функци получается 3-значное число
// // При N = 300'000;
// // set: 1226 milliseconds
// // unordered_set: 10655 milliseconds
// struct PlateHasher {
//     size_t operator()(const Plate& p) const {
//         return p.Number;
//     }
// };


// // на выходе этой хэш-функци получается уже 5-значное число (в отличие от 3-значного)
// // При N = 300'000;
// // set: 1215 milliseconds
// // unordered_set: 265 milliseconds
// struct PlateHasher {
//     size_t operator()(const Plate& p) const {
//         return p.Number * 100 + p.Region;
//     }
// };

// // При N = 300'000;
// // et: 1212 milliseconds
// // unordered_set: 139 milliseconds
struct PlateHasher {
    size_t operator()(const Plate& p) const {
        size_t first = static_cast<size_t>((p.Number * 100 + p.Region)) * 1'000'000u;
        size_t second = (((p.C1 - 'A') * 100) + (p.C2 - 'A')) * 100 + (p.C3 - 'A');
        size_t result = first + second;
        return result;
    }
};