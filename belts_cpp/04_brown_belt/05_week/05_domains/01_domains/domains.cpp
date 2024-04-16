#include "domains.h"

#include <algorithm>  // std::upper_bound
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>  // std::move

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

vector<string> ReadDomains(istream& in) {
    size_t count;
    in >> count;
    in.ignore(1);

    vector<string> domains;
    string domain;
    for (size_t i = 0; i < count; ++i) {
        getline(in, domain);
        reverse(domain.begin(), domain.end());
        domains.push_back(std::move(domain));
    }

    return domains;
}

bool IsBannedDomain(const string& domain, const vector<string>& banned_domains) {
    const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
    return it != begin(banned_domains) && IsSubdomainReversed(domain, *prev(it));
}

