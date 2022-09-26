#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename VectorElementType>
ostream& operator<<(ostream& os, const vector<VectorElementType>& vec) {
    bool isFirstElement = true;
    os << "[";
    for (const auto& elm : vec) {
        if (isFirstElement) {
            os << elm;
        } else {
            os << ", "s << elm;
        }
        isFirstElement = false;
    }
    os << "]";

    return os;
}

template <typename SetlementType>
ostream& operator<<(ostream& os, const set<SetlementType>& _set_) {
    bool isFirstElement = true;
    os << "{";
    for (const auto& elm : _set_) {
        if (isFirstElement) {
            os << elm;
        } else {
            os << ", "s << elm;
        }
        isFirstElement = false;
    }
    os << "}";

    return os;
}

template <typename KeyType, typename ValueType>
ostream& operator<<(ostream& os, const map<KeyType, ValueType>& _map_) {
    bool isFirstElement = true;
    os << "{";
    for (const auto& [key, value] : _map_) {
        if (isFirstElement) {
            os << key << ": " << value;
        } else {
            os << ", "s << key << ": " << value;
            ;
        }
        isFirstElement = false;
    }
    os << "}";

    return os;
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

vector<int> TakeEvens(const vector<int>& numbers) {
    vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}

vector<int> TakePositives(const vector<int>& numbers) {
    vector<int> positives;
    for (int x : numbers) {
        // Ошибка допущена намеренно, чтобы продемонстрировать вывод при несработавшем AssertEqual
        if (x >= 0) {
            positives.push_back(x);
        }
    }
    return positives;
}

map<string, int> TakeAdults(const map<string, int>& people) {
    map<string, int> adults;
    for (const auto& [name, age] : people) {
        if (age >= 18) {
            adults[name] = age;
        }
    }
    return adults;
}

bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        ++i;
    }
    return true;
}

set<int> TakePrimes(const set<int>& numbers) {
    set<int> primes;
    for (int number : numbers) {
        if (IsPrime(number)) {
            primes.insert(number);
        }
    }
    return primes;
}

int main() {
    {
        const set<int> numbers = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        const set<int> expected_primes = {2, 3, 5, 7, 11, 13};
        ASSERT_EQUAL(TakePrimes(numbers), expected_primes);
    }

    // {
    //     // intentionally wrong expected_primes
    //     const set<int> numbers = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    //     const set<int> expected_primes = {2, 3, 5, 7, 11, 13, 111};
    //     ASSERT_EQUAL(TakePrimes(numbers), expected_primes);
    // }

    {
        const map<string, int> people = {{"Ivan"s, 19}, {"Sergey"s, 16}, {"Alexey"s, 18}};
        const map<string, int> expected_adults = {{"Alexey"s, 18}, {"Ivan"s, 19}};
        ASSERT_EQUAL(TakeAdults(people), expected_adults);
    }

    {
        const vector<int> numbers = {3, 2, 1, 0, 3, 6};
        const vector<int> expected_evens = {2, 0, 6};
        ASSERT_EQUAL(TakeEvens(numbers), expected_evens);
    }
}