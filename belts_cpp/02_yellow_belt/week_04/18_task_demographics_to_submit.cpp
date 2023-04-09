#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

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