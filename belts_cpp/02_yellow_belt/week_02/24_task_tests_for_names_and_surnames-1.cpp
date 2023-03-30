#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/*
==================== Тесты для класса Person ====================
Задача

В первом курсе «Белый пояс по С++» у нас была задача «Имена и фамилии — 1».
В ней надо было разработать класс Person, поддерживающий историю изменений человеком своих фамилии и имени.
В данной задаче вам надо разработать юнит-тесты на реализацию класса Person.
При разработке тестов учитывайте ограничения, которые накладывает на класс Person условие задачи «Имена и фамилии — 1».

Начать работу вы можете с шаблона, который содержит наш фреймворк юнит-тестов и заготовку класса.

test_person.cpp
Условие задачи «Имена и фамилии — 1»

Реализуйте класс для человека, поддерживающий историю изменений человеком своих фамилии и имени.

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
    // добавить факт изменения имени на first_name в год year
  }
  void ChangeLastName(int year, const string& last_name) {
    // добавить факт изменения фамилии на last_name в год year
  }
  string GetFullName(int year) {
    // получить имя и фамилию по состоянию на конец года year
  }
private:
  // приватные поля
};

Считайте, что в каждый год может произойти не более одного изменения фамилии и не более одного изменения имени. При этом с течением времени могут открываться всё новые факты из прошлого человека, поэтому года́ в последовательных вызовах методов ChangeLastName и ChangeFirstName не обязаны возрастать.

Гарантируется, что все имена и фамилии непусты.

Строка, возвращаемая методом GetFullName, должна содержать разделённые одним пробелом имя и фамилию человека по состоянию на конец данного года.

    Если к данному году не случилось ни одного изменения фамилии и имени, верните строку "Incognito".

    Если к данному году случилось изменение фамилии, но не было ни одного изменения имени, верните "last_name with unknown first name".

    Если к данному году случилось изменение имени, но не было ни одного изменения фамилии, верните "first_name with unknown last name".

Примечание

Перед отправкой вашего файла убедитесь, что он не содержит класса Person. В противном случае вы получите ошибку компиляции. Наша тестирующая система умеет игнорировать функции в файлах (поэтому, например, в задаче "Тесты для функции IsPalindrom" необязательно удалять функцию IsPalindrom), но не умеет это делать для классов.

*/

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

class Person {
   public:
    // Вы можете вставлять сюда различные реализации класса, чтобы проверить,
    // что ваши тесты пропускают корректный код и ловят некорректный

    void ChangeFirstName(int year, const string& first_name) {
        _year2name[year] = first_name;
    }

    void ChangeLastName(int year, const string& last_name) {
        _year2surname[year] = last_name;
    }

    string GetFullName(int year) {
        std::ostringstream oss;
        if ((_year2name.empty() && _year2surname.empty()) || (IsNameUnknown(year) && IsSurnameUnknown(year))) {
            oss << "Incognito"s;
        } else if (!IsNameUnknown(year) && IsSurnameUnknown(year)) {
            oss << GetName(year) << " with unknown last name"s;
        } else if (IsNameUnknown(year) && !IsSurnameUnknown(year)) {
            oss << GetLastName(year) << " with unknown first name"s;
        } else {
            oss << GetName(year) << " "s << GetLastName(year);
        }

        return oss.str();
    }

   private:
    std::map<int, std::string> _year2name;
    std::map<int, std::string> _year2surname;

    bool IsNameUnknown(int year) {
        return _year2name.empty() ? true : year < _year2name.begin()->first;
    }

    bool IsSurnameUnknown(int year) {
        return _year2surname.empty() ? true : year < _year2surname.begin()->first;
    }

    // ограничение: год должен быть не меньше, чем минимальное значение ключа в "_year2name"
    string GetName(int year) {
        auto it = _year2name.lower_bound(year);
        return (_year2name.count(year)) ? it->second : std::prev(it)->second;
    }

    // ограничение: год должен быть не меньше, чем минимальное значение ключа в "_year2surname"
    string GetLastName(int year) {
        auto it = _year2surname.lower_bound(year);
        return (_year2surname.count(year)) ? it->second : std::prev(it)->second;
    }
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
