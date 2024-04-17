#include "test_runner.h"

#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <utility>

using namespace std;

vector<string> ReadDomainsIntoVector(istream& in = std::cin) {
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

unordered_set<string> ReadDomainsInUnorderedSet(istream& in = std::cin) {
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

vector<string> GetDomainStatuses(const vector<string>& domains_to_check,
                                 const unordered_set<string>& banned_domains) {
    std::vector<string> statuses;
    statuses.reserve(domains_to_check.size());

    for (const string& domain_to_check: domains_to_check) {
        if (banned_domains.count(domain_to_check)) {
            statuses.emplace_back("Bad");
            continue;
        }

        string_view s = domain_to_check;
        bool is_banned = false;

        for (size_t dot_pos = s.find('.'); dot_pos != string::npos; dot_pos = s.find('.')) {
            s.remove_prefix(dot_pos + 1);
            if (banned_domains.count(std::string(s))) {
                is_banned = true;
                break;
            }
        }

        statuses.emplace_back((is_banned)
                              ? "Bad"
                              : "Good");
    }

    return statuses;
}

