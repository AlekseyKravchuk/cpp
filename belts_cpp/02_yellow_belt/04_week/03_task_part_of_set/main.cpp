#include <iostream>
#include <set>
#include <sstream>
#include <vector>

#include "tests.h"
#include "find_greatest.h"

using namespace std;


int main() {
    TestAll();

    for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
        cout << x << " ";
    }
    cout << endl;

    string to_find = "Python";
    cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
    
    return 0;
}