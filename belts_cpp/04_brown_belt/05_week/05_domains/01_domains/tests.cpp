#include "test_runner.h"
#include "domains.h"

#include <algorithm>

using namespace std;

string ReverseString(const string& str) {
    return {str.rbegin(), str.rend()};
}

void TestReadDomains() {
    { // Test empty
        istringstream in("0");
        vector<string> actual = ReadDomains(in);
        vector<string> expected = {};

        ASSERT_EQUAL(actual.size(), expected.size());
        ASSERT_EQUAL(actual, expected);
    }

    { // Test one domain
        istringstream in("1\nya.ru");
        vector<string> actual = ReadDomains(in);
        vector<string> expected = {"ur.ay"};

        ASSERT_EQUAL(actual.size(), expected.size());
        ASSERT_EQUAL(actual, expected);
    }

    { // Test two domains
        istringstream in("2\nya.ru\ngoogle.com\n");
        vector<string> actual = ReadDomains(in);
        vector<string> expected = {"ur.ay", "moc.elgoog"};

        ASSERT_EQUAL(actual.size(), expected.size());
        ASSERT_EQUAL(actual, expected);
    }
}

void TestIsSubdomain() {
    ASSERT(IsSubdomainReversed("", ""));

    ASSERT(IsSubdomainReversed(ReverseString("ru"),
                               ReverseString("ru")));

    ASSERT(IsSubdomainReversed(ReverseString("ya.ru"),
                               ReverseString("ya.ru")));

    ASSERT(IsSubdomainReversed(ReverseString("ya.ru"),
                               ReverseString("ru")));


    ASSERT(IsSubdomainReversed(ReverseString("yz.abc.de"),
                               ReverseString("abc.de")));

    ASSERT(IsSubdomainReversed(ReverseString("wx.yz.abc.de"),
                               ReverseString("abc.de")));

    ASSERT(!IsSubdomainReversed(ReverseString("a.x"),
                                ReverseString("b.x")));

    ASSERT(!IsSubdomainReversed(ReverseString("a.x"),
                                ReverseString("ab.x")));

    ASSERT(!IsSubdomainReversed(ReverseString("a.x"),
                                ReverseString("aa.x")));

    ASSERT(!IsSubdomainReversed(ReverseString("a.x"),
                                ReverseString("b.aa.x")));

    ASSERT(IsSubdomainReversed(ReverseString("abc.de"),
                                ReverseString("yz.abc.de")));

    ASSERT(IsSubdomainReversed(ReverseString("abc.de"),
                                ReverseString("wx.yz.abc.de")));
}

void TestIsSubdomainReversed() {
    vector<pair<string, string>> positive_tests = {
            {"",             ""},
            {"ur",           "ur"},
            {"ur.ay",        "ur.ay"},
            {"ur.ay",        "ur"},
            {"ed.cba.zy",    "ed.cba"},
            {"ed.cba.zy.xw", "ed.cba"},
    };

    vector<pair<string, string>> negative_tests = {
            {"x.a",    "x.b"},
            {"x.a",    "x.ba"},
            {"x.a",    "x.aa"},
            {"x.a",    "x.aa.b"},
            {"ed.cba", "ed.cba.zy"},
            {"ed.cba", "ed.abc.zy.xw"},
    };

    for (auto& [left, right] : negative_tests) {
        left = ReverseString(left);
        right = ReverseString(right);
    }

    for (const auto& [subdomain, domain]: positive_tests) {
        ASSERT(IsSubdomainReversed(subdomain, domain));
    }

    for (const auto& [subdomain, domain]: negative_tests) {
        ASSERT(!IsSubdomainReversed(subdomain, domain));
    }
}

void TestIsBannedDomain() {
    const vector<string> banned_domains = [] {
        vector<string> tmp = {"ya.ru", "maps.me", "m.ya.ru", "com"};
        for (string& domain: tmp) {
            reverse(begin(domain), end(domain));
        }
        sort(begin(tmp), end(tmp));
        return tmp;
    }();

    vector<string> positive_tests = {"ya.ru", "ya.com", "m.maps.me", "moscow.m.ya.ru", "maps.com"};
    vector<string> negative_tests = {"maps.ru", "ya.ya"};

    for (auto domain: positive_tests) {
        reverse(begin(domain), end(domain));
        ASSERT(IsBannedDomain(domain, banned_domains));
    }

    for (auto domain: negative_tests) {
        reverse(begin(domain), end(domain));
        ASSERT(!IsBannedDomain(domain, banned_domains));
    }
}

void RunTests() {
    TestRunner tr;
    RUN_TEST(tr, TestReadDomains);
    RUN_TEST(tr, TestIsSubdomain);
    RUN_TEST(tr, TestIsSubdomainReversed);
    RUN_TEST(tr, TestIsBannedDomain);
}
