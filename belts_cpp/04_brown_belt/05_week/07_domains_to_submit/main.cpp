#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <utility>

using namespace std;

vector<string> ReadDomains(istream& in = cin) {
    size_t count;
    in >> count;
    vector<string> domains;
    for (size_t i = 0; i < count; ++i) {
        string domain;
        in >> domain;
        domains.emplace_back(std::move(domain));
    }

    return domains;
}

int main() {
    const std::unordered_set<string> forbidden = []() {
        std::unordered_set<string> result;
        for (const string& domain: ReadDomains(cin)) {
            result.emplace(domain);
        }
        return result;
    }();
    const vector<string> domains_to_check = ReadDomains(cin);

    for (const string& domain: domains_to_check) {
        if (forbidden.find(domain) != forbidden.end()) {
            cout << "Bad\n";
            continue;
        }

        string_view subdomain = domain;
        bool is_forbiden = false;
        std::size_t dot_pos = subdomain.find(".");
        while (dot_pos != string::npos) {
            subdomain = subdomain.substr(dot_pos + 1);
            if (forbidden.find(string(subdomain)) != forbidden.end()) {
                is_forbiden = true;
                break;
            }
            dot_pos = subdomain.find(".");
        }

        if (is_forbiden)
            cout << "Bad\n";
        else
            cout << "Good\n";
    }
    return 0;
}
