#include "domains.h"
#include "tests.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>

using namespace std;

int main() {
    RunTests();

    // IILE idiom
    const vector<string> banned_domains = [] {
        vector<string> banned = ReadDomains();
        sort(begin(banned), end(banned));
        return banned;
    }();

    const vector<string> domains_to_check = ReadDomains();

    for (const string& domain: domains_to_check) {
        if (IsBannedDomain(domain, banned_domains)) {
            cout << "Bad" << endl;
        } else {
            cout << "Good" << endl;
        }
    }

    return 0;
}

