#include "domains.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

bool IsSubdomainReversed(string_view subdomain, string_view domain) {
    size_t min_len = std::min(domain.size(), subdomain.size());
    size_t i = 0;
    while (i < min_len) {
        if(subdomain[i] != domain[i]) {
            return false;
        }
        ++i;
    }

    return min_len == domain.size() || subdomain[min_len] == '.';
}

vector<string> ReadDomains(istream& in) {
  size_t count;
  in >> count;
  in.ignore(1);

  vector<string> domains;
  for (size_t i = 0; i < count; ++i) {
    string domain;
    getline(in, domain);
    domains.push_back(domain);
  }
  return domains;
}

bool IsBannedDomain(string_view domain, const vector<string>& banned_domains) {
  const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
  return it != begin(banned_domains) && IsSubdomainReversed(domain, *prev(it));
}

