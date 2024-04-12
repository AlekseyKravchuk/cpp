#include "trip_manager.h"

#include <iostream>

using namespace std;

int main() {
    try {
        TripManager tm;
        auto trip = tm.Book({});
        tm.Cancel(trip);
    } catch (...) {
        cerr << "Exception in function main.\n";
    }


    return 0;
}
