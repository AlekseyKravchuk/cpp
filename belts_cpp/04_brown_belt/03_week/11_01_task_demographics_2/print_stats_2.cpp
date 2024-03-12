#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>  // std::distance
#include <map>
#include <numeric>  // std::partial_sum
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename Iterator>
class IteratorRange {
   public:
    IteratorRange(Iterator begin, Iterator end)
        : _first(begin), _last(end) {
    }

    Iterator begin() const {
        return _first;
    }

    Iterator end() const {
        return _last;
    }

   private:
    Iterator _first;
    Iterator _last;
};

template <typename Collection>
auto Head(Collection& c, size_t top) {
    return IteratorRange{c.begin(),
                         next(c.begin(), min(top, c.size()))};
}

struct Person {
    string name;
    int age;
    int income;
    bool is_male;
};

// ========================== class PersonsDB ==========================
class PersonsDB {
   public:
    PersonsDB(istream& input);
    int NumGreaterOrEqualMaturityAge(int maturity_age) const;
    int GetTotalIncome(int top_n) const;
    optional<string> GetMostPopularName(bool is_male) const;

   private:
    const vector<Person> _persons;  // sorted by age
    vector<int> _cumulative_incomes;
    optional<string> _popular_male_name;
    optional<string> _popular_female_name;

   private:
    // ============================ METHODS ============================
    vector<Person> GetPersonsSortedByAge(istream& input);
    vector<int> GetCumulativeIncomes();
    void SetPopularNames();

    template <typename Iterator>
    optional<string> FindPopularName(IteratorRange<Iterator> iter_range);
};

PersonsDB::PersonsDB(istream& input)
    : _persons(GetPersonsSortedByAge(input)),
      _cumulative_incomes(GetCumulativeIncomes()) {
    SetPopularNames();
}

vector<Person> PersonsDB::GetPersonsSortedByAge(istream& input) {
    size_t count;
    input >> count;

    vector<Person> result(count);

    char gender{0};
    for (Person& p : result) {
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = (gender == 'M');
    }

    sort(result.begin(),
         result.end(),
         [](const Person& lhs, const Person& rhs) {
             return lhs.age < rhs.age;  // sort in ascending order
         });

    return result;
}

vector<int> PersonsDB::GetCumulativeIncomes() {
    vector<int> cumulative_incomes(_persons.size());

    transform(_persons.begin(), _persons.end(),
              cumulative_incomes.begin(),
              [](const Person& person) {
                  return person.income;
              });
    sort(cumulative_incomes.begin(), cumulative_incomes.end(), greater<int>());  // sort in descending order
    partial_sum(cumulative_incomes.begin(), cumulative_incomes.end(),
                cumulative_incomes.begin());

    return cumulative_incomes;
}

void PersonsDB::SetPopularNames() {
    vector<Person> persons_copy{_persons};
    auto border = partition(persons_copy.begin(), persons_copy.end(),
                            [](const Person& p) {
                                return p.is_male;
                            });

    IteratorRange males{persons_copy.begin(), border};
    IteratorRange females{border, persons_copy.end()};

    _popular_male_name = FindPopularName(males);
    _popular_female_name = FindPopularName(females);
}

template <typename Iterator>
optional<string> PersonsDB::FindPopularName(IteratorRange<Iterator> iter_range) {
    if (iter_range.begin() == iter_range.end()) {
        return nullopt;
    }

    sort(iter_range.begin(), iter_range.end(),
         [](const Person& lhs, const Person& rhs) {
             return lhs.name < rhs.name;
         });

    const string* most_popular_name = &iter_range.begin()->name;
    int count = 1;
    for (auto it = iter_range.begin(); it != iter_range.end();) {
        auto same_name_end = find_if_not(
            it,
            iter_range.end(),
            [it](const Person& p) {
                return p.name == it->name;
            });
        auto cur_name_count = std::distance(it, same_name_end);

        if (cur_name_count > count) {
            count = cur_name_count;
            most_popular_name = &(it->name);
        }

        it = same_name_end;
    }

    return *most_popular_name;
}

int PersonsDB::NumGreaterOrEqualMaturityAge(int maturity_age) const {
    auto lb = lower_bound(begin(_persons),
                          end(_persons),
                          maturity_age,
                          [](const Person& person, int age) {
                              return person.age < age;
                          });
    return distance(lb, end(_persons));
}

int PersonsDB::GetTotalIncome(int top_n) const {
    if (top_n < 1) {
        return 0;
    }

    if (top_n > _cumulative_incomes.size()) {
        top_n = _cumulative_incomes.size();
    }

    return _cumulative_incomes[top_n - 1];
}

optional<string> PersonsDB::GetMostPopularName(bool is_male) const {
    return is_male
               ? _popular_male_name
               : _popular_female_name;
}
// ======================= END of class PersonsDB =======================

enum class Command : char {
    AGE,
    WEALTHY,
    POPULAR_NAME
};

template <typename T>
map<string, T> myMap;

template <>
map<string, Command> myMap<Command> = {
    {"AGE", Command::AGE},
    {"WEALTHY", Command::WEALTHY},
    {"POPULAR_NAME", Command::POPULAR_NAME},
};

template <typename T>
T stringToEnum(const string& str) {
    return static_cast<T>(myMap<T>.at(str));
}

void ProcessCommands(const PersonsDB& db, istream& input, ostream& out) {
    for (string command; input >> command;) {
        switch (stringToEnum<Command>(command)) {
            case Command::AGE: {
                int maturity_age;
                input >> maturity_age;
                out << "There are " << db.NumGreaterOrEqualMaturityAge(maturity_age)
                    << " adult people for maturity age " << maturity_age << '\n';
                break;
            }
            case Command::WEALTHY: {
                int count;
                input >> count;
                out << "Top-" << count << " people have total income "
                    << db.GetTotalIncome(count) << '\n';
                break;
            }
            case Command::POPULAR_NAME: {
                char gender;
                input >> gender;
                bool is_male = (gender == 'M');

                if (auto popular_name = db.GetMostPopularName(is_male); popular_name) {
                    out << "Most popular name among people of gender " << gender << " is "
                        << *popular_name << '\n';
                } else {
                    out << "No people of gender " << gender << '\n';
                }
                break;
            }
            default:
                throw std::logic_error("Unknown command in input stream: " + command);
                break;
        }
    }
}

void Test1() {
    istringstream input{
        R"(11
Ivan 25 1000 M
Olga 30 623 W
Sergey 24 825 M
Maria 42 1254 W
Mikhail 15 215 M
Oleg 18 230 M
Denis 53 8965 M
Maxim 37 9050 M
Ivan 47 19050 M
Ivan 17 50 M
Olga 23 550 W
AGE 18
AGE 25
WEALTHY 5
POPULAR_NAME M
POPULAR_NAME W
)"};

    PersonsDB db(input);
    ostringstream output;
    ProcessCommands(db, input, output);

    const string expected{
        R"(There are 9 adult people for maturity age 18
There are 6 adult people for maturity age 25
Top-5 people have total income 39319
Most popular name among people of gender M is Ivan
Most popular name among people of gender W is Olga
)"};

    ASSERT_EQUAL(expected, output.str());
}

void RunTests() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
}

int main() {
    // Основной проблемой исходного решения было то, что в нём случайно изменялись исходные данные.
    // Код после рефакторинга гарантирует, что изменения исходных данных не произойдёт.

    RunTests();
    PersonsDB db(cin);
    ProcessCommands(db, cin, cout);
}
