#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Person {
    string name;
    int age;
    int income;
    bool is_male;
};

int main() {
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

    int count;
    input >> count >> ws;

    vector<Person> people;
    people.resize(count);

    char gender{0};
    for (Person& p : people) {
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = (gender == 'M');
    }

    return 0;
}
