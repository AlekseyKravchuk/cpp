#include <iostream>
#include <iterator>
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
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
   public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
        std::cerr << "==================== ALL TESTS SUCCESSFULLY PASSED ===================="s << std::endl;
    }

   private:
    int fail_count = 0;
};


void Test_Incognito() {
    {
        Person person;
        int year = 1900;
        AssertEqual("Incognito"s, person.GetFullName(year), "Empty names and surnames"s);
    }

    {
        Person person;
        person.ChangeFirstName(1965, "Polina");
        person.ChangeLastName(1967, "Sergeeva");
        int year = 1900;
        AssertEqual("Incognito"s, person.GetFullName(year), "Incognito==Incognito"s);
    }
}

void Test_FirstName_with_Unknown_LastName() {
    {
        Person person;
        person.ChangeFirstName(1965, "Polina"s);
        person.ChangeFirstName(1961, "Olga"s);
        person.ChangeFirstName(1967, "Alisa"s);
        int year = 1969;
        AssertEqual("Alisa with unknown last name"s, person.GetFullName(year), "LastName is empty"s);
    }

    {
        Person person;
        person.ChangeFirstName(1965, "Polina"s);
        person.ChangeLastName(1970, "Sergeeva"s);
        int year = 1968;
        AssertEqual("Polina with unknown last name"s, person.GetFullName(year), "Year is before LastName keys begin"s);
    }

    {
        Person person;
        person.ChangeFirstName(1965, "Polina"s);
        person.ChangeFirstName(1961, "Marina"s);
        person.ChangeLastName(1970, "Sergeeva");
        int year = 1964;
        AssertEqual("Marina with unknown last name"s, person.GetFullName(year), "FirstName with Unknown, year = 1964"s);
    }

    {
        Person person;
        person.ChangeFirstName(1965, "Polina"s);
        person.ChangeFirstName(1961, "Marina"s);
        person.ChangeLastName(1970, "Sergeeva");
        int year = 1969;
        AssertEqual("Polina with unknown last name"s, person.GetFullName(year), "FirstName with Unknown, year = 1969"s);
    }

    {
        Person person;
        person.ChangeFirstName(1965, "Polina"s);
        person.ChangeFirstName(1961, "Marina"s);
        int year = 1969;
        AssertEqual("Polina with unknown last name"s, person.GetFullName(year), "FirstName with Unknown"s);
    }
}

void Test_LastName_with_Unknown_FirstName() {
    {
        Person person;
        person.ChangeLastName(1965, "Sergeeva"s);
        person.ChangeLastName(1961, "Mylneva"s);
        person.ChangeLastName(1967, "Svetova"s);
        int year = 1969;
        AssertEqual("Svetova with unknown first name"s, person.GetFullName(year), "Svetova, year = 1969, names is empty"s);
    }

    {
        Person person;
        person.ChangeLastName(1965, "Sergeeva"s);
        person.ChangeLastName(1961, "Mylneva"s);
        person.ChangeLastName(1967, "Svetova"s);
        person.ChangeFirstName(1980, "Alena"s);
        int year = 1963;
        AssertEqual("Mylneva with unknown first name"s, person.GetFullName(year), "Svetova, year = 1963"s);
    }

    {
        Person person;
        person.ChangeLastName(1965, "Sergeeva"s);
        person.ChangeLastName(1961, "Mylneva"s);
        person.ChangeLastName(1967, "Svetova"s);
        person.ChangeFirstName(1980, "Alena"s);
        int year = 1961;
        AssertEqual("Mylneva with unknown first name"s, person.GetFullName(year), "Svetova, year = 1961"s);
    }

    {
        Person person;
        person.ChangeLastName(1965, "Sergeeva"s);
        person.ChangeLastName(1961, "Mylneva"s);
        person.ChangeLastName(1967, "Svetova"s);
        person.ChangeFirstName(1980, "Alena"s);
        int year = 1979;
        AssertEqual("Svetova with unknown first name"s, person.GetFullName(year), "Svetova, year = 1961"s);
    }
}

void Test_FullName() {
    {
        Person person;
        person.ChangeLastName(1965, "Sergeeva"s);
        person.ChangeLastName(1961, "Mylneva"s);
        person.ChangeLastName(1967, "Svetova"s);
        person.ChangeFirstName(1980, "Alena"s);
        int year = 1988;
        AssertEqual("Alena Svetova"s, person.GetFullName(year), "Alena Svetova, year = 1988"s);
    }

    {
        Person person;
        person.ChangeLastName(1965, "Voronina"s);
        person.ChangeLastName(1961, "Mylneva"s);
        person.ChangeLastName(1967, "Sergeeva"s);
        person.ChangeFirstName(1967, "Alena"s);
        int year = 1988;
        AssertEqual("Alena Sergeeva"s, person.GetFullName(year), "Alena Sergeeva, year = 1988"s);
    }
}

void TestAll() {
    // объявляем объект класса "TestRunner"
    TestRunner runner;
    runner.RunTest(Test_Incognito, "Test_Incognito"s);
    runner.RunTest(Test_FirstName_with_Unknown_LastName, "Test_FirstName_with_Unknown_LastName"s);
    runner.RunTest(Test_LastName_with_Unknown_FirstName, "Test_LastName_with_Unknown_FirstName"s);
    runner.RunTest(Test_FullName, "Test_FullName"s);
}  // по этой закрывающей скобке вызывается деструктор "TestRunner::~TestRunner()", в котором выполняется анализ переменной "fail_count"

int main() {
    TestAll();

    Person person;

    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : {1900, 1965, 1990}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeLastName(1968, "Volkova");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    return 0;
}
