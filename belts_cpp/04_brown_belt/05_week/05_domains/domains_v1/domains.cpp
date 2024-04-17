#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

using namespace std;

// Good job! (Max time used: 0.25/3.00, max preprocess time used: 0/None, max memory used: 118263808/536870912.)

bool IsSubDomain(const string& subdomain, const string& domain) {
    size_t i = 0;
    size_t j = 0;
    while (i < subdomain.size() && j < domain.size()) {
        if (subdomain[i++] != domain[j++]) {
            return false;
        }
    }

    if (subdomain.size() != domain.size()) {
        return (j == domain.size() && subdomain[i] == '.') ||
               (i == subdomain.size() && domain[j] == '.');
    }

    return true;
}

vector<string> ReadBannedDomains(istream& is = std::cin) {
    size_t count;
    is >> count;
    vector<string> result;
    result.reserve(count);
    string current_domain;

    for (size_t i = 0; i < count; ++i) {
        is >> current_domain;
        reverse(begin(current_domain), end(current_domain));
        result.push_back(std::move(current_domain));
    }
    sort(begin(result), end(result));

    size_t insert_pos = 0;
    for (string& domain: result) {
        if (insert_pos == 0 || !IsSubDomain(domain, result[insert_pos - 1])) {
            swap(result[insert_pos++], domain);
        }
    }
    result.resize(insert_pos);
    return result;
}


vector<string> ReadDomains(istream& is = std::cin) {
    size_t count;
    is >> count;
    vector<string> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        string domain;
        is >> domain;
        reverse(begin(domain), end(domain));
        result.push_back(std::move(domain));
    }

    return result;
}


string PrintDomainStatus(vector<string>& banned_domains, const string& domain_to_check) {
    const auto it = upper_bound(banned_domains.begin(),
                                banned_domains.end(),
                                domain_to_check);
    return (it != banned_domains.begin()
            && IsSubDomain(domain_to_check, *prev(it)))
           ? "Bad"
           : "Good";
}

void SimpleTest() {
    std::istringstream input_banned{R"(
1
ya.ru
)"};

    std::istringstream input_to_check{R"(
5
ya.ru
ya.com
m.ya.ru
m.mail.ya.ru
ru
)"};

    vector<string> banned_domains = ReadBannedDomains(input_banned);
    vector<string> domains_to_check = ReadDomains(input_to_check);
    vector<string> expected = {"Bad", "Good", "Bad", "Bad", "Good"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(PrintDomainStatus(banned_domains, domains_to_check[i]),
                     expected[i]);
    }
}

void TestSameLong() {
    std::istringstream input_banned{R"(
3
ya.ru
m.ya.ru
moscow.spb.ru
)"};

    std::istringstream input_to_check{R"(
5
m.maps.me
m.ya.ru
b.ya.ru
m.ya
ya.ya)"};

    vector<string> banned_domains = ReadBannedDomains(input_banned);
    vector<string> domains_to_check = ReadDomains(input_to_check);
    vector<string> expected = {"Good", "Bad", "Bad", "Good", "Good"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(PrintDomainStatus(banned_domains, domains_to_check[i]),
                     expected[i]);
    }
}

void TestSame() {
    std::istringstream input_banned{R"(
1
m.spb.ru.com)"};

    std::istringstream input_to_check{R"(
6
com
ru.com
spb.ru.com
m.spb.ru.com
s.spb.ru.com
s.m.spb.ru.com
)"};

    vector<string> banned_domains = ReadBannedDomains(input_banned);
    vector<string> domains_to_check = ReadDomains(input_to_check);
    vector<string> expected = {"Good", "Good", "Good", "Bad", "Good", "Bad"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(PrintDomainStatus(banned_domains, domains_to_check[i]),
                     expected[i]);
    }
}

void TestAllBanned() {
    std::istringstream input_banned{R"(
2
com
ya.ru
)"};

    std::istringstream  input_to_check{R"(
7
com
ru.com
spb.ru.com
s.spb.ru.com
s.m.spb.ru.com
spb.ya.ru
m.x.z.ya.ru
)"};

    vector<string> banned_domains = ReadBannedDomains(input_banned);
    vector<string> domains_to_check = ReadDomains(input_to_check);
    vector<string> expected = {"Bad", "Bad", "Bad", "Bad", "Bad", "Bad", "Bad"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(PrintDomainStatus(banned_domains, domains_to_check[i]),
                     expected[i]);
    }
}

void TestCheckSubDomains() {
    std::istringstream input_banned{R"(
8
ya.com
spb.com
m.com
m.ya.com
ya.m.com
ya.ya.com
yaya.com
ya.ru.com
)"};

    std::istringstream input_to_check{R"(
8
com
ru.com
spb.ru.com
m.spb.com
s.spb.com
spb.m.com
spb.yaya.com
moscow.ya.ru
)"};

    vector<string> banned_domains = ReadBannedDomains(input_banned);
    vector<string> domains_to_check = ReadDomains(input_to_check);
    vector<string> expected = {"Good", "Good", "Good", "Bad", "Bad", "Bad", "Bad", "Good"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(PrintDomainStatus(banned_domains, domains_to_check[i]),
                     expected[i]);
    }
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, SimpleTest);
    RUN_TEST(tr, TestSameLong);
    RUN_TEST(tr, TestSame);
    RUN_TEST(tr, TestAllBanned);
    RUN_TEST(tr, TestCheckSubDomains);
}

int main() {
    TestAll();

    vector<string> banned_domains = ReadBannedDomains();
    vector<string> domains_to_check = ReadDomains();

    for (const auto& domain_to_check: domains_to_check) {
        cout << PrintDomainStatus(banned_domains, domain_to_check) << '\n';
    }

    return 0;
}