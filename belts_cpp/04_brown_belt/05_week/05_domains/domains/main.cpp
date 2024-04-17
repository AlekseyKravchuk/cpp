#include "tests.h"
#include "domains.h"

#include <string>
#include <unordered_set>

using namespace std;

int main() {
    TestAll();

    const unordered_set<string> banned_domains = ReadDomainsInUnorderedSet();
    const vector<string> domains_to_check = ReadDomainsIntoVector();

    for (const string& status : GetDomainStatuses(domains_to_check, banned_domains)) {
        cout << status << '\n';
    }

    return 0;
}
