#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <utility>

using namespace std;

// Good job! (Max time used: 0.15/3.00, max preprocess time used: 0/None, max memory used: 118267904/536870912.)

vector<string> ReadDomainsIntoVector(istream& in = cin) {
    size_t count;
    in >> count;
    vector<string> domains;
    domains.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        string domain;
        in >> domain;
        domains.emplace_back(std::move(domain));
    }

    return domains;
}

unordered_set<string> ReadDomainsInUnorderedSet(istream& in = cin) {
    size_t count;
    in >> count;
    unordered_set<string> domains;

    for (size_t i = 0; i < count; ++i) {
        string domain;
        in >> domain;
        domains.insert(std::move(domain));
    }

    return domains;
}

int main() {
    const unordered_set<string> banned_domains = ReadDomainsInUnorderedSet();
    const vector<string> domains_to_check = ReadDomainsIntoVector();

    for (const string& domain: domains_to_check) {
        if (banned_domains.find(domain) != banned_domains.end()) {
            cout << "Bad\n";
            continue;
        }

        string_view subdomain = domain;
        bool is_forbiden = false;

        std::size_t dot_pos = subdomain.find(".");
        while (dot_pos != string::npos) {
            subdomain = subdomain.substr(dot_pos + 1);
            if (banned_domains.find(string(subdomain)) != banned_domains.end()) {
                is_forbiden = true;
                break;
            }
            dot_pos = subdomain.find(".");
        }

        if (is_forbiden) {
            cout << "Bad\n";
        } else {
            cout << "Good\n";
        }
    }

    return 0;
}
