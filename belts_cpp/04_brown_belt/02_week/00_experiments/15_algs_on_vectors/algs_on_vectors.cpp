#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>  // std::partial_sum

#include "print.h"

using namespace std;

struct Person {
    string name;
    int age;
    int income;
    bool is_male;
};

int main() {
    // // vector<int> v = {1, 2, 2, 2, 3, 4, 5, 5, 5, 5, 6, 6, 7, 7, 7, 7, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 11, 12, 12, 13};
    // vector<int> v = {1, 2, 2, 2, 3, 4, 5, 5, 5, 5, 6, 6, 7};
    // int number = 5;
    // auto lb = lower_bound(v.begin(), v.end(), number);
    // cout << "number of elements greater or equal to " << number << ": " << distance(lb, end(v)) << " items" << endl;

    // ================================================================================================================
    vector<int> v = {1, 2, 2, 2, 3, 4, 5, 5, 5, 5, 6, 6};
    partial_sum(v.begin(), v.end(), v.begin());
    cout << "v after partial_sum: " << v << endl;

//     istringstream input{
//         R"(11
// Ivan 25 1000 M
// Olga 30 623 W
// Sergey 24 825 M
// Maria 42 1254 W
// Mikhail 15 215 M
// Oleg 18 230 M
// Denis 53 8965 M
// Maxim 37 9050 M
// Ivan 47 19050 M
// Ivan 17 50 M
// Olga 23 550 W
// AGE 18
// AGE 25
// WEALTHY 5
// POPULAR_NAME M
// POPULAR_NAME W
// )"};

//     int count;
//     input >> count >> ws;

//     vector<Person> people;
//     people.resize(count);

//     char gender{0};
//     for (Person& p : people) {
//         input >> p.name >> p.age >> p.income >> gender;
//         p.is_male = (gender == 'M');
//     }

//     sort(people.begin(),
//          people.end(),
//          [](const Person& lhs, const Person& rhs) {
//              return lhs.age < rhs.age;
//          });

//     // хотим вывести количество людей, возраст зрелости которых больше, либо равен заданному
//     int maturity_age = 19;
//     size_t num_of_people = distance(lower_bound(people.begin(),
//                                                 people.end(),
//                                                 maturity_age,
//                                                 [](const Person& lhs, int age) {
//                                                     return lhs.age < age;
//                                                 }),
//                                     people.end());

//     cout << "number of people whose age is greater or equal to " << maturity_age << " years old: " << num_of_people << endl;

    return 0;
}
