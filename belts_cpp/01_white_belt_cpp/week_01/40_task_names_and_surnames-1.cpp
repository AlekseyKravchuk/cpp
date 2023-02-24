#include <algorithm>
#include <iostream>
#include <iterator>  // std::prev
#include <map>
#include <sstream>
#include <string>
#include <utility>  // std::pair

using namespace std::literals;

/*
Задание по программированию «Имена и фамилии — 1»

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

 

Считайте, что в каждый год может произойти не более одного изменения фамилии и не более одного изменения имени.
При этом с течением времени могут открываться всё новые факты из прошлого человека,
поэтому года́ в последовательных вызовах методов ChangeLastName и ChangeFirstName не обязаны возрастать.

Гарантируется, что все имена и фамилии непусты.

Строка, возвращаемая методом GetFullName, должна содержать разделённые одним пробелом имя и фамилию человека по состоянию на конец данного года.
    - Если к данному году не случилось ни одного изменения фамилии и имени, верните строку "Incognito".
    - Если к данному году случилось изменение фамилии, но не было ни одного изменения имени, верните "last_name with unknown first name".
    - Если к данному году случилось изменение имени, но не было ни одного изменения фамилии, верните "first_name with unknown last name".

Пример
Код:

int main() {
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


Вывод:
Incognito
Polina with unknown last name
Polina Sergeeva
Polina Sergeeva
Appolinaria Sergeeva
Polina Volkova
Appolinaria Volkova
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

   private:
    std::map<int, std::string> _year2name;
    std::map<int, std::string> _year2surname;
};

int main() {
    Person person;

    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : {1900, 1965, 1990}) {
        std::cout << person.GetFullName(year) << std::endl;
    }

    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : {1969, 1970}) {
        std::cout << person.GetFullName(year) << std::endl;
    }

    person.ChangeLastName(1968, "Volkova");
    for (int year : {1969, 1970}) {
        std::cout << person.GetFullName(year) << std::endl;
    }

    return 0;
}
