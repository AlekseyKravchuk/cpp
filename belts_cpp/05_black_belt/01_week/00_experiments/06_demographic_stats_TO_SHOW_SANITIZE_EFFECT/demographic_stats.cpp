#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
  Компилируем программу:
clang++-17 -g -std=c++17 demographic_stats.cpp -o demographic_stats -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address
  Запускаем скомпилированную программу:
ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-17/bin/llvm-symbolizer ./demographic_stats
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

istream& operator>>(istream& in, Person& p) {
    int age, gender;
    bool is_employed;

    if (in >> age >> gender >> is_employed) {
        p = {age, Gender{gender}, is_employed};
    }

    return in;
}

template<typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
     // // Преднамеренно комментируем проверку входного диапазона для демонстрации работы санитайзера
     // if (range_begin == range_end) {
     //     return 0;
     // }

    vector<typename InputIt::value_type> v_copy(range_begin, range_end);
    auto middle = begin(v_copy) + v_copy.size() / 2;

    nth_element(begin(v_copy), middle, end(v_copy),
                [](const Person& lhs, const Person& rhs) {
                    return lhs.age < rhs.age;
                }
    );

    return middle->age;  // here we are trying to dereference NULL pointer (iterator)
}

void PrintStats(vector<Person> persons) {
    // Преобразуем порядок людей к следующему:
    //                  persons
    //                 /      \
    //          females        males
    //         /       \     /      \
    //      empl.  unempl. empl.   unempl.
    auto females_end = partition(begin(persons), end(persons),
                                 [](const Person& p) {
                                     return p.gender == Gender::FEMALE;
                                 }
    );

    auto employed_females_end = partition(begin(persons), females_end,
                                          [](const Person& p) {
                                              return p.is_employed;
                                          }
    );

    auto employed_males_end = partition(females_end, end(persons),
                                        [](const Person& p) {
                                            return p.is_employed;
                                        }
    );

    // Теперь интересующие нас группы находятся в векторе непрерывно
    cout << "Median age = "
         << ComputeMedianAge(begin(persons), end(persons)) << endl;
    cout << "Median age for females = "
         << ComputeMedianAge(begin(persons), females_end) << endl;
    cout << "Median age for males = "
         << ComputeMedianAge(females_end, end(persons)) << endl;
    cout << "Median age for employed females = "
         << ComputeMedianAge(begin(persons), employed_females_end) << endl;
    cout << "Median age for unemployed females = "
         << ComputeMedianAge(employed_females_end, females_end) << endl;
    cout << "Median age for employed males = "
         << ComputeMedianAge(females_end, employed_males_end) << endl;
    cout << "Median age for unemployed males = "
         << ComputeMedianAge(employed_males_end, end(persons)) << endl;
}

int main() {
//    vector<Person> persons = {
//            {31, Gender::MALE,   false},
//            {40, Gender::FEMALE, true},
//            {24, Gender::MALE,   true},
//            {20, Gender::FEMALE, true},
//            {80, Gender::FEMALE, false},
//            {78, Gender::MALE,   false},
//            {10, Gender::FEMALE, false},
//            {55, Gender::MALE,   true},
//    };

    size_t persons_count = 0;
    cin >> persons_count;
    vector<Person> persons(persons_count);

    for (size_t i = 0; i < persons_count; ++i) {
        Person& person = persons[i];
        cin >> person;
    }


    PrintStats(persons);

    return 0;
}
