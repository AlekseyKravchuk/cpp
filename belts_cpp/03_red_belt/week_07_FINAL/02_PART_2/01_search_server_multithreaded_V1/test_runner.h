#pragma once

#include "profile.h"

#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator<<(ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator<<(ostream& os, const list<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator<<(ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template <class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (!(t == u)) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

inline void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
  public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        {
            std::cerr << std::endl;
            LOG_DURATION(test_name);
            try {
                func();
                cerr << std::left << std::setw(20) << test_name + ':' << " OK" << endl;
            } catch (exception& e) {
                ++fail_count;
                cerr << test_name + ':' << " fail: " << e.what() << endl;
            } catch (...) {
                ++fail_count;
                cerr << "Unknown exception caught" << endl;
            }
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

  private:
    int fail_count = 0;
};

#ifndef UNIQ_ID

#define GENERATE(x) a_local_uniq_id__##x
#define PROXY(x) GENERATE(x)
#define UNIQ_ID PROXY(__LINE__)

#endif

#define ASSERT_EQUAL(x, y)                       \
    {                                            \
        std::ostringstream UNIQ_ID;              \
        UNIQ_ID << #x << " != " << #y << ", ";   \
        UNIQ_ID << __FILE__ << ": " << __LINE__; \
        AssertEqual(x, y, UNIQ_ID.str());        \
    }

#define ASSERT(x)                                \
    {                                            \
        std::ostringstream UNIQ_ID;              \
        UNIQ_ID << #x << " is false, ";          \
        UNIQ_ID << __FILE__ << ": " << __LINE__; \
        AssertEqual(x, true, UNIQ_ID.str());     \
    }

// tr - object of class TestRunner
// # - change name of variable into string
#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)
