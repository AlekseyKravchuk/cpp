#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Отладочные макроопределения - это специальные директивы препроцессора, которые включают проверки инвариантов в
 * стандартной библиотеке (выход за границы вектора, валидность диапазона итераторов и пр.).
 * При отсутствии этих макроопределений программа работает так же скоростью.
 * Когда же мы включаем эти отладочные макроопределения, у нас начинают срабатывать эти проверки.
 * При этом программа начинает работать ДОЛЬШЕ.
 * Ошибки в случае использования отладочных макроопределений выявляются на этапе выполнения (в RUNTIME'е).
 * Это замедляет выполнение программы.
 */

#ifdef MY_DEBUG_CONFIGURATION
enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

template<typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }

    vector<typename InputIt::value_type> v_copy(range_begin, range_end);
    auto middle = begin(v_copy) + v_copy.size() / 2;

//    nth_element(begin(v_copy), middle, end(v_copy),
//                [](const Person& lhs, const Person& rhs) {
//                    return lhs.age < rhs.age;
//                }
//    );

    // ПРЕДНАМЕРЕННО вносим ошибку: меняем местами итераторы "middle" и "end(v_copy)"
    // за счет использования отладочных макроопределений препроцессора (DEBUGGING FLAGS)
    // "-D_GLIBCXX_DEBUG" и "-D_GLIBCXX_DEBUG_PEDANTIC" выявляем ошибку при запуске под отладчиком
    nth_element(begin(v_copy), end(v_copy), middle,
                [](const Person& lhs, const Person& rhs) {
                    return lhs.age < rhs.age;
                }
    );

    return middle->age;
}
#endif  // end of MY_DEBUG_CONFIGURATION

void PrintStats(vector<Person> persons) {
    // Преобразуем порядок людей к следующему:
    //                  persons
    //                 /      \
    //          females        males
    //         /       \     /      \
    //      empl.  unempl. empl.   unempl.
    auto females_end = partition(
            begin(persons), end(persons), [](const Person& p) {
                return p.gender == Gender::FEMALE;
            }
    );
    auto employed_females_end = partition(
            begin(persons), females_end, [](const Person& p) {
                return p.is_employed;
            }
    );
    auto employed_males_end = partition(
            females_end, end(persons), [](const Person& p) {
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
    vector<Person> persons = {
            {31, Gender::MALE,   false},
            {40, Gender::FEMALE, true},
            {24, Gender::MALE,   true},
            {20, Gender::FEMALE, true},
            {80, Gender::FEMALE, false},
            {78, Gender::MALE,   false},
            {10, Gender::FEMALE, false},
            {55, Gender::MALE,   true},
    };

    PrintStats(persons);

    return 0;
}
