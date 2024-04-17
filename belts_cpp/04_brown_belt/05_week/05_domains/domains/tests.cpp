#include "test_runner.h"
#include "domains.h"

#include <unordered_set>

using namespace std;

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

    const unordered_set <string> banned_domains = ReadDomainsInUnorderedSet(input_banned);
    const vector<string> domains_to_check = ReadDomainsIntoVector(input_to_check);
    const std::vector<string> statuses = GetDomainStatuses(domains_to_check, banned_domains);
    const std::vector<string> expected_statuses = {"Bad", "Good", "Bad", "Bad", "Good"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(statuses[i], expected_statuses[i]);
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
ya.ya
)"};

    const unordered_set <string> banned_domains = ReadDomainsInUnorderedSet(input_banned);
    const vector<string> domains_to_check = ReadDomainsIntoVector(input_to_check);
    const std::vector<string> statuses = GetDomainStatuses(domains_to_check, banned_domains);
    const std::vector<string> expected_statuses = {"Good", "Bad", "Bad", "Good", "Good"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(statuses[i], expected_statuses[i]);
    }
}

void TestSame() {
    std::istringstream input_banned{R"(
1
m.spb.ru.com
)"};

    std::istringstream input_to_check{R"(
6
com
ru.com
spb.ru.com
m.spb.ru.com
s.spb.ru.com
s.m.spb.ru.com
)"};

    const unordered_set <string> banned_domains = ReadDomainsInUnorderedSet(input_banned);
    const vector<string> domains_to_check = ReadDomainsIntoVector(input_to_check);
    const std::vector<string> statuses = GetDomainStatuses(domains_to_check, banned_domains);
    const std::vector<string> expected_statuses = {"Good", "Good", "Good", "Bad", "Good", "Bad"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(statuses[i], expected_statuses[i]);
    }
}

void TestAllBanned() {
    std::istringstream input_banned{R"(
1
ya.ru
)"};

    std::istringstream input_to_check{R"(
3
mail.ya.ru
spb.mail.ya.ru
center.spb.mail.ya.ru
)"};

    const unordered_set <string> banned_domains = ReadDomainsInUnorderedSet(input_banned);
    const vector<string> domains_to_check = ReadDomainsIntoVector(input_to_check);
    const std::vector<string> statuses = GetDomainStatuses(domains_to_check, banned_domains);
    const std::vector<string> expected_statuses = {"Bad", "Bad", "Bad"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(statuses[i], expected_statuses[i]);
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

    const unordered_set <string> banned_domains = ReadDomainsInUnorderedSet(input_banned);
    const vector<string> domains_to_check = ReadDomainsIntoVector(input_to_check);
    const std::vector<string> statuses = GetDomainStatuses(domains_to_check, banned_domains);
    const std::vector<string> expected_statuses = {"Good", "Good", "Good", "Bad", "Bad", "Bad", "Bad", "Good"};

    for (size_t i = 0; i < domains_to_check.size(); ++i) {
        ASSERT_EQUAL(statuses[i], expected_statuses[i]);
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