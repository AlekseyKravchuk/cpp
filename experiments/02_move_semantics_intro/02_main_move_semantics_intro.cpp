#include "tests.h"

int main() {
    // ProfileCopyAssignment_vs_Moving();
    // ProfileInsertingToSet_Copying_vs_Moving();
    ProfileAddingKeyValuePairToEmptyMap_Copying_vs_Moving();

    // map::operator[], with BOTH variables: for key and for value: 1860 ms
    // map::operator[], variable ONLY for key: 1389 ms
    // map::operator[], WITHOUT variables: 930 ms

    return 0;
}
