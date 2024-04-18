#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <utility>  // std::pair, std::move

using namespace std;

template<typename It>
class Range {
  public:
    Range(It begin, It end) : _begin(begin), _end(end) {}

    It begin() const { return _begin; }

    It end() const { return _end; }

  private:
    It _begin;
    It _end;
};

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delim = " ") {
    const size_t pos = s.find(delim);
    if (pos == string::npos) {
        return {s, nullopt};
    } else {
        return {s.substr(0, pos), s.substr(pos + delim.length())};
    }
}

vector<string_view> Split(string_view s, string_view delim = " ") {
    vector<string_view> parts;
    if (s.empty()) {
        return parts;
    }
    while (true) {
        const auto [lhs, rhs_opt] = SplitTwoStrict(s, delim);
        parts.push_back(lhs);
        if (!rhs_opt) {
            break;
        }
        s = *rhs_opt;
    }
    return parts;
}


class Domain {
  public:
    explicit Domain(string_view text) {
        vector<string_view> parts = Split(text, ".");
        _parts_reversed.assign(rbegin(parts), rend(parts));
    }

    size_t GetPartCount() const {
        return _parts_reversed.size();
    }

    auto GetParts() const {
        return Range(rbegin(_parts_reversed), rend(_parts_reversed));
    }

    auto GetReversedParts() const {
        return Range(begin(_parts_reversed), end(_parts_reversed));
    }

    bool operator==(const Domain& other) const {
        return _parts_reversed == other._parts_reversed;
    }

  private:
    vector<string> _parts_reversed;
};

ostream& operator<<(ostream& out, const Domain& domain) {
    bool first = true;
    for (const string_view part: domain.GetParts()) {
        if (!first) {
            out << '.';
        } else {
            first = false;
        }
        out << part;
    }

    return out;
}

// domain is subdomain of itself
bool IsSubdomain(const Domain& subdomain, const Domain& domain) {
    const auto subdomain_reversed_parts = subdomain.GetReversedParts();
    const auto domain_reversed_parts = domain.GetReversedParts();

    return subdomain.GetPartCount() >= domain.GetPartCount()
           && equal(begin(domain_reversed_parts),
                    end(domain_reversed_parts),
                    begin(subdomain_reversed_parts));
}

bool IsSubOrSuperDomain(const Domain& lhs, const Domain& rhs) {
    return lhs.GetPartCount() >= rhs.GetPartCount()
           ? IsSubdomain(lhs, rhs)
           : IsSubdomain(rhs, lhs);
}

class DomainChecker {
  public:
    template<typename InputIt>
    DomainChecker(InputIt domains_begin, InputIt domains_end) {
        _sorted_domains.reserve(distance(domains_begin, domains_end));
        for (const Domain& domain: Range(domains_begin, domains_end)) {
            _sorted_domains.push_back(&domain);
        }
        sort(begin(_sorted_domains), end(_sorted_domains), IsDomainLess);
        _sorted_domains = AbsorbSubdomains(std::move(_sorted_domains));
    }

    // Check if candidate is subdomain of some domain
    bool IsSubdomain(const Domain& candidate) const {
        const auto it = upper_bound(_sorted_domains.begin(),
                                    _sorted_domains.end(),
                                    &candidate,
                                    IsDomainLess);
        if (it == begin(_sorted_domains)) {
            return false;
        }

        return ::IsSubdomain(candidate, **prev(it));
    }

  private:
    vector<const Domain*> _sorted_domains;

    static bool IsDomainLess(const Domain* lhs, const Domain* rhs) {
        const auto lhs_reversed_parts = lhs->GetReversedParts();
        const auto rhs_reversed_parts = rhs->GetReversedParts();

        return lexicographical_compare(
                begin(lhs_reversed_parts), end(lhs_reversed_parts),
                begin(rhs_reversed_parts), end(rhs_reversed_parts)
        );
    }

    static vector<const Domain*> AbsorbSubdomains(vector<const Domain*> domains) {
        domains.erase(unique(begin(domains),
                             end(domains),
                             [](const Domain* lhs, const Domain* rhs) {
                                 return IsSubOrSuperDomain(*lhs, *rhs);
                             }),
                      end(domains));
        return domains;
    }
};


vector<Domain> ReadDomains(istream& in = cin) {
    vector<Domain> domains;

    size_t count;
    in >> count;
    domains.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        string domain_as_str;
        in >> domain_as_str;
        domains.emplace_back(domain_as_str);
    }
    return domains;
}

vector<bool> CheckDomains(const vector<Domain>& banned_domains, const vector<Domain>& domains_to_check) {
    const DomainChecker checker(begin(banned_domains), end(banned_domains));

    vector<bool> check_results;
    check_results.reserve(domains_to_check.size());
    for (const Domain& domain_to_check: domains_to_check) {
        check_results.push_back(!checker.IsSubdomain(domain_to_check));
    }

    return check_results;
}

void PrintCheckResults(const vector<bool>& check_results, ostream& out_stream = cout) {
    for (const bool check_result: check_results) {
        out_stream << (check_result ? "Good" : "Bad") << "\n";
    }
}

// ============================= TESTS =============================
void TestSplitWithExcessEmpty() {
    ASSERT_EQUAL(
            Split("ya.ru", "."),
            vector<string_view>({"ya", "ru"})
    );
}

void TestDomainReversedParts() {
    const Domain domain("ya.ru");
    const auto reversed_parts_range = domain.GetReversedParts();
    const vector<string_view> reversed_parts(
            begin(reversed_parts_range), end(reversed_parts_range)
    );
    ASSERT_EQUAL(
            reversed_parts,
            vector<string_view>({"ru", "ya"})
    );
}

void TestIsSubdomainNonReflexive() {
    ASSERT_EQUAL(
            IsSubdomain(Domain("ru"), Domain("ru")),
            true
    );
}

void TestIsSubdomainMixedArgs() {
    ASSERT_EQUAL(IsSubdomain(Domain("m.ya.ru"), Domain("ya.ru")),
                 true);
}

void TestDomainCheckerNoAbsorbation() {
    const vector<Domain> domains = {
            Domain("ya.ru"),
            Domain("m.ya.ru")
    };
    ASSERT_EQUAL(
            DomainChecker(begin(domains), end(domains)).IsSubdomain(Domain("pets.ya.ru")),
            true
    );
}

void TestCheckDomainsInvertBool() {
    ASSERT_EQUAL(
            CheckDomains(vector({Domain("ya.ru")}), vector({Domain("m.ya.ru")})),
            vector({false})
    );
}

void TestPrintCheckResultsGoodOrBad() {
    ostringstream stream;
    PrintCheckResults(vector({true, false}), stream);
    ASSERT_EQUAL(stream.str(), "Good\nBad\n");
}

void TestReadDomainsExcessEmpty() {
    stringstream stream;
    stream << 1 << '\n' << "ya.ru";
    ASSERT_EQUAL(
            ReadDomains(stream),
            vector({Domain("ya.ru")})
    );
}

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitWithExcessEmpty);
    RUN_TEST(tr, TestDomainReversedParts);
    RUN_TEST(tr, TestIsSubdomainNonReflexive);
    RUN_TEST(tr, TestIsSubdomainMixedArgs);
    RUN_TEST(tr, TestDomainCheckerNoAbsorbation);
    RUN_TEST(tr, TestCheckDomainsInvertBool);
    RUN_TEST(tr, TestPrintCheckResultsGoodOrBad);
    RUN_TEST(tr, TestReadDomainsExcessEmpty);
}
// ========================== END OF TESTS ==========================

int main() {
    TestAll();

    return 0;
}