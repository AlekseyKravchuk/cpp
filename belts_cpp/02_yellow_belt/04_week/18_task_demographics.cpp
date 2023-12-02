#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

/*
Демографические показатели

В этой задаче вам необходимо вычислить различные демографические показатели для группы людей. Человек представляется структурой Person:

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};

Тип Gender определён следующим образом:

enum class Gender {
  FEMALE,
  MALE
};

Вам необходимо написать функцию PrintStats, получающую вектор людей, вычисляющую и выводящую медианный возраст для каждой из следующих групп людей:

    все люди;

    все женщины;

    все мужчины;

    все занятые женщины;

    все безработные женщины;

    все занятые мужчины;

    все безработные мужчины.

Все 7 чисел нужно вывести в строгом соответствии с форматом (см. пример).

void PrintStats(vector<Person> persons);

Принимая вектор по значению (а не по константной ссылке), вы получаете возможность модифицировать его копию произвольным образом и тем самым проще произвести вычисления.
Подсказка

Используйте алгоритм partition.

Вычисление медианного возраста

Для вычисления медианного возраста группы людей вы должны использовать функцию ComputeMedianAge:

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end);

Эту функцию не нужно реализовывать самостоятельно: мы реализовали её за вас и автоматически добавим к каждому вашему решению.

Функцию ComputeMedianAge можно вызвать и для пустого набора людей: её результат в этом случае и нужно считать медианным возрастом пустого набора людей.

Пример кода

// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

void PrintStats(vector<Person> persons);

int main() {
  vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}

Вывод

Median age = 40
Median age for females = 40
Median age for males = 55
Median age for employed females = 40
Median age for unemployed females = 80
Median age for employed males = 55
Median age for unemployed males = 78

Пояснение

В этой задаче вам надо прислать на проверку файл с реализацией функции PrintStats. Этот файл не должен содержать определения функции ComputeMedianAge и типов Person и Gender. В противном случае вы получите ошибку компиляции.

Гарантируется, что типы Person и Gender объявлены в точности так же, как в коде выше.
*/
enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }

    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(
        begin(range_copy), middle, end(range_copy),
        [](const Person& lhs, const Person& rhs) {
            return lhs.age < rhs.age;
        });
    return middle->age;
}

void PrintStats(vector<Person> persons) {
    std::cout << "Median age = "s << ComputeMedianAge(persons.begin(), persons.end()) << std::endl;

    auto it_female_male = std::partition(persons.begin(), persons.end(),
                                         [](const Person& p) {
                                             return p.gender == Gender::FEMALE;
                                         });
    std::cout << "Median age for females = "s
              << ComputeMedianAge(persons.begin(), it_female_male) << std::endl;
    std::cout << "Median age for males = "s
              << ComputeMedianAge(it_female_male, persons.end()) << std::endl;

    auto it_employed_unemployed_females = std::partition(persons.begin(), it_female_male,
                                                         [](const Person& p) {
                                                             return p.is_employed == true;
                                                         });
    std::cout << "Median age for employed females = "s
              << ComputeMedianAge(persons.begin(), it_employed_unemployed_females) << std::endl;
    std::cout << "Median age for unemployed females = "s
              << ComputeMedianAge(it_employed_unemployed_females, it_female_male) << std::endl;

    auto it_employed_unemployed_males = std::partition(it_female_male, persons.end(),
                                                       [](const Person& p) {
                                                           return p.is_employed == true;
                                                       });
    std::cout << "Median age for employed males = "s
              << ComputeMedianAge(it_female_male, it_employed_unemployed_males) << std::endl;
    std::cout << "Median age for unemployed males = "s
              << ComputeMedianAge(it_employed_unemployed_males, persons.end()) << std::endl;
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    PrintStats(persons);
    return 0;
}