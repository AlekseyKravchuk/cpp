#include "print.h"

#include <string>

using namespace std;

int main() {
    unordered_map<string, int> u_map;

    u_map.insert({"01", 22});
    u_map.insert({"03", 11});
    u_map.insert({"04", 33});

    u_map.emplace("08", 55);

    // it:       iterator to the inserted pair of elements: pair<KeyType, ValueType>{key, value};
    // inserted: a bool denoting whether the insertion took place (true if insertion happened, false if it did not)
    // auto [it, inserted] = u_map.emplace("03", 999);
    auto [it, inserted] = u_map.try_emplace("03", 999);
    if (!inserted) {
        cout << "Element (" << it->first << ", " << it->second << ") wasn't inserted because such a key already exists." << endl;
    }

    cout << u_map << endl;
    
    return 0;
}
 