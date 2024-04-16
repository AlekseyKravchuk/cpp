#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

bool IsSubdomainReversed(const string& subdomain, const string& domain) {
    int i = 0; int j = 0;
    while (i < subdomain.size() && j < domain.size()) {
        if (subdomain[i++] != domain[j++]) {
            return false;
        }
    }

    return (subdomain.size() == domain.size()) ||
           (j == domain.size() && subdomain[i] == '.') ||
           (i == subdomain.size() && domain[j] == '.');
}

vector<string> ReadDomains(istream& in = std::cin) {
    size_t count;
    in >> count;
    in.ignore(1);

    vector<string> domains;
    string domain;
    for (size_t i = 0; i < count; ++i) {
        getline(in, domain);
        reverse(domain.begin(), domain.end());
        domains.push_back(domain);
    }

    return domains;
}

bool IsBannedDomain(const string& domain, const vector<string>& banned_domains) {
    const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
    return it != begin(banned_domains) && IsSubdomainReversed(domain, *prev(it));
}

int main() {
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

