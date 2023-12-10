#pragma once

#include <iomanip>
#include <iostream>
#include <tuple>

// структура для номерного знака
struct Plate {
    char C1;
    int Number;
    char C2;
    char C3;
    int Region;
};

std::ostream& operator<<(std::ostream& out, const Plate& plate) {
    out << plate.C1;
    out << std::setw(3) << std::setfill('0') << plate.Number;
    out << plate.C2;
    out << plate.C3;
    out << std::setw(2) << std::setfill('0') << plate.Region;

    return out;
}

bool operator<(const Plate& lhs, const Plate& rhs) {
    return std::tuple(lhs.C1, lhs.Number, lhs.C2, lhs.C3, lhs.Region) <
           std::tuple(rhs.C1, rhs.Number, rhs.C2, rhs.C3, rhs.Region);
}

bool operator==(const Plate& lhs, const Plate& rhs) {
    return std::tuple(lhs.C1, lhs.Number, lhs.C2, lhs.C3, lhs.Region) ==
           std::tuple(rhs.C1, rhs.Number, rhs.C2, rhs.C3, rhs.Region);
}