#include <algorithm>
#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

/*

Задание по программированию «Имена и фамилии — 2»

 

Дополните класс из предыдущей задачи «Имена и фамилии — 1» методом GetFullNameWithHistory:

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
  string GetFullNameWithHistory(int year) {
    // получить все имена и фамилии по состоянию на конец года year
  }
private:
  // приватные поля
};

 

В отличие от метода GetFullName, метод GetFullNameWithHistory должен вернуть не только последние имя и фамилию к концу данного года, но ещё и все предыдущие имена и фамилии в обратном хронологическом порядке. Если текущие факты говорят о том, что человек два раза подряд изменил фамилию или имя на одно и то же, второе изменение при формировании истории нужно игнорировать.

Для лучшего понимания формата см. примеры.
Пример 1
Код

int main() {
  Person person;

  person.ChangeFirstName(1900, "Eugene");
  person.ChangeLastName(1900, "Sokolov");
  person.ChangeLastName(1910, "Sokolov");
  person.ChangeFirstName(1920, "Evgeny");
  person.ChangeLastName(1930, "Sokolov");
  cout << person.GetFullNameWithHistory(1940) << endl;
  
  return 0;
}

 

Вывод

Evgeny (Eugene) Sokolov

 
Пример 2
Код

int main() {
  Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1990, "Polina");
  person.ChangeLastName(1990, "Volkova-Sergeeva");
  cout << person.GetFullNameWithHistory(1990) << endl;
  
  person.ChangeFirstName(1966, "Pauline");
  cout << person.GetFullNameWithHistory(1966) << endl;
  
  person.ChangeLastName(1960, "Sergeeva");
  for (int year : {1960, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeLastName(1961, "Ivanova");
  cout << person.GetFullNameWithHistory(1967) << endl;
  
  return 0;
}

Вывод:
Incognito
Polina with unknown last name
Polina Sergeeva
Polina Sergeeva
Appolinaria (Polina) Sergeeva
Polina Volkova (Sergeeva)
Appolinaria (Polina) Volkova (Sergeeva)
Polina (Appolinaria, Polina) Volkova-Sergeeva (Volkova, Sergeeva)
Pauline (Polina) with unknown last name
Sergeeva with unknown first name
Pauline (Polina) Sergeeva
Pauline (Polina) Sergeeva (Ivanova, Sergeeva)
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
    void ChangeFirstName(int year, const std::string& first_name) {
        _year2name[year] = first_name;
    }

    void ChangeLastName(int year, const std::string& last_name) {
        _year2surname[year] = last_name;
    }

    std::string GetFullName(int year) {
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

    std::string GetFullNameWithHistory(int year) {
        // получить все имена и фамилии по состоянию на конец года year
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
    std::map<int, std::string> _year2name;
    std::map<int, std::string> _year2surname;
};

int main() {
    Person person;

    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : {1900, 1965, 1990}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : {1969, 1970}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    person.ChangeLastName(1968, "Volkova");
    for (int year : {1969, 1970}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    person.ChangeFirstName(1990, "Polina");
    person.ChangeLastName(1990, "Volkova-Sergeeva");
    std::cout << person.GetFullNameWithHistory(1990) << std::endl;

    person.ChangeFirstName(1966, "Pauline");
    std::cout << person.GetFullNameWithHistory(1966) << std::endl;

    person.ChangeLastName(1960, "Sergeeva");
    for (int year : {1960, 1967}) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }

    person.ChangeLastName(1961, "Ivanova");
    std::cout << person.GetFullNameWithHistory(1967) << std::endl;

    return 0;
}
