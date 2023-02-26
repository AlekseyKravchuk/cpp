#include <algorithm>
#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

/*
Задание по программированию «Имена и фамилии — 3»
 
Дополните класс Person из задачи «Имена и фамилии — 2» конструктором, позволяющим задать имя и фамилию человека при рождении, а также сам год рождения. Класс не должен иметь конструктора по умолчанию.

При получении на вход года, который меньше года рождения:

    методы GetFullName и GetFullNameWithHistory должны отдавать "No person";

    методы ChangeFirstName и ChangeLastName должны игнорировать запрос.

Кроме того, необходимо объявить константными все методы, которые по сути ими являются.
Пример
 
Код:

int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}

Вывод:
No person
Polina Sergeeva
Appolinaria (Polina) Sergeeva
Appolinaria (Polina) Ivanova (Sergeeva)


*/

std::string GetValueByYear(const std::map<int, std::string>& m, int year) {
    if (!m.empty()) {
        int min_year = m.begin()->first;
        int max_year = m.rbegin()->first;

        if (year < min_year) {
            return {};
        }

        if (year > max_year) {
            year = max_year;
        }

        if (m.count(year)) {
            return m.at(year);
        } else {
            return std::prev(m.lower_bound(year))->second;
        }

    } else {
        return {};
    }
}

std::string GetHistoryByYear(const std::map<int, std::string>& m, int year) {
    if (!m.empty()) {
        int min_year = m.begin()->first;
        int max_year = m.rbegin()->first;
        std::string s;

        if (year < min_year) {
            return {};
        }

        if (year > max_year) {
            year = max_year;
        }

        decltype(m.begin()) it;
        it = (m.count(year)) ? m.find(year) : std::prev(m.lower_bound(year));

        auto num_hops = std::distance(m.begin(), it);
        std::vector<std::string> history;
        history.reserve(num_hops + 1);
        history.push_back(it->second);

        for (auto i = 0; i < num_hops; ++i) {
            auto prev_pushed = it->second;
            it = std::prev(it);
            if (prev_pushed != it->second) {
                history.push_back(it->second);
            }
        }

        if (history.size() > 1) {
            s = history[0] + " (";

            bool isFirst = true;
            for (auto i = 1; i < history.size(); ++i) {
                if (isFirst) {
                    s += history[i];
                    isFirst = false;
                } else {
                    s += ", " + history[i];
                }
            }
            s += ")"s;
        } else {
            s = history[0];
        }

        return s;
    } else {
        return {};
    }
}

class Person {
   public:
    Person(const std::string& first_name, const std::string& last_name, int year)
        : _first_name(first_name), _last_name(last_name), _year(year) {
            ChangeFirstName(_year, _first_name);
            ChangeLastName(_year, _last_name);
        }

    void ChangeFirstName(int year, const std::string& first_name) {
        _year2name[year] = first_name;
    }

    void ChangeLastName(int year, const std::string& last_name) {
        _year2surname[year] = last_name;
    }

    std::string GetFullName(int year) const {
        if (year < _year) {
            return "No person"s;
        }

        // получить имя и фамилию по состоянию на конец года year
        std::string name = ::GetValueByYear(_year2name, year);
        std::string surname = ::GetValueByYear(_year2surname, year);

        if (name.empty()) {
            if (surname.empty()) {
                return "Incognito"s;
            } else {
                return surname + " with unknown first name"s;
            }
        } else {
            if (surname.empty()) {
                return name + " with unknown last name"s;
            } else {
                return name + " "s + surname;
            }
        }
    }

    std::string GetFullNameWithHistory(int year) const {
        // получить все имена и фамилии по состоянию на конец года year
        if (year < _year) {
            return "No person"s;
        }

        std::string name = ::GetHistoryByYear(_year2name, year);
        std::string surname = ::GetHistoryByYear(_year2surname, year);

        if (name.empty()) {
            if (surname.empty()) {
                return "Incognito"s;
            } else {
                return surname + " with unknown first name"s;
            }
        } else {
            if (surname.empty()) {
                return name + " with unknown last name"s;
            } else {
                return name + " "s + surname;
            }
        }
    }

   private:
    std::string _first_name{};
    std::string _last_name{};
    int _year{};

    std::map<int, std::string> _year2name{};
    std::map<int, std::string> _year2surname{};
};

int main() {
    Person person("Polina", "Sergeeva", 1960);
    for (int year : {1959, 1960}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    person.ChangeFirstName(1965, "Appolinaria");
    person.ChangeLastName(1967, "Ivanova");
    for (int year : {1965, 1967}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    return 0;
}
