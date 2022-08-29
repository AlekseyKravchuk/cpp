#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
                       const map<string, int>& max_amount,
                       map<string, int>& shelter) {
    int accepted_dogs_count = count_if(new_dogs.begin(), new_dogs.end(),
                                       [&max_amount, &shelter](const string& dog_breed) {
                                           if (max_amount.at(dog_breed) - shelter[dog_breed] > 0) {
                                               shelter[dog_breed] += 1;
                                               return true;
                                           } else {
                                               return false;
                                           }
                                       });
    return accepted_dogs_count;
}

int main() {
    const vector<string> new_dogs = {"landseer"s, "otterhound"s, "otterhound"s, "otterhound"s, "pointer"s};
    const map<string, int> max_amount = {{"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 4}, {"pointer"s, 7}};
    map<string, int> shelter = {{"landseer"s, 1}, {"otterhound"s, 2}, {"pekingese"s, 2}, {"pointer"s, 3}};

    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl;

    return 0;
}