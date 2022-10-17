/*
#include "carpenter.h"
// Обратите внимание, что square_calculation.h включен в файл carpenter.cpp,
// а файл wall.h включен в carpenter.h
// Позже мы разберемся, по какому принципу выбирать,
// куда и что лучше включать
#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) / 2;
}
*/

#include "carpenter.h"

#include "square_calculation.h"

int Carpenter::CalcShelves(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) / 2;
}

void Carpenter::InstallDoor(Wall& wall, const Builder& builder) const {
    // Hold the door, builder! Hold the door!
    builder.HoldDoor();
    wall.SetDoorInstalled();
}
