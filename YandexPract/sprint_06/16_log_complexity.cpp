#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

template <typename F>
int FindFloorMy(int numberOfFloors, F drop) {
    int low = 1;
    int high = numberOfFloors;

    while (low != high) {
        int mid = low + ((high - low) / 2);

         if (drop(mid)) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    return low;
}

template <typename F>
int FindFloor(int n, F drop) {
    int low = 1;
    int high = n;
    do {
        int mid = low + (high - low) / 2;
        if (drop(mid)) {
            high = mid;
        } else {
            low = mid + 1;
        }
    } while (low != high);

    return low;
}

void StressTest() {
    int N = pow(2, 3);
    int countMy = 0;
    int count = 0;

    while (true) {
        int targetFloor = rand() % N;

        int foundMy = FindFloorMy(N, [targetFloor, &countMy](int currentFloor) {
            ++countMy;
            return currentFloor >= targetFloor;
        });

        int found = FindFloor(N, [targetFloor, &count](int currentFloor) {
            ++count;
            return currentFloor >= targetFloor;
        });

        if (foundMy == found) {
            cout << "OK" << endl;
        } else {
            cout << "\nERROR FOUND for case: " << "N = "s << N << ", targetFloor = " << targetFloor << endl;
            cout << "foundMy: Found floor "s << foundMy << " after "s << countMy << " drops"s << endl;
            cout << "found: Found floor "s << found << " after "s << count << " drops"s << endl;
            break;
        }
    }
}

int main() {
    #define _GLIBCXX_DEBUG 1

    #ifdef  _GLIBCXX_DEBUG
    StressTest();
    #endif  // _GLIBCXX_DEBUG
    
    int numberOfFloors, targetFloor;
    cout << "Enter number of floors and target floor number: "s << endl;
    cin >> numberOfFloors >> targetFloor;

    int count = 0;

    int found = FindFloorMy(numberOfFloors, [targetFloor, &count](int currentFloor) {
        ++count;
        return currentFloor >= targetFloor;
    });

    cout << "Found floor "s << found << " after "s << count << " drops"s;

    return 0;
}