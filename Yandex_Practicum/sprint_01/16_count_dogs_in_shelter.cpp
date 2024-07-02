#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
                       const map<string, int>& max_amount,
                       map<string, int>& shelter) {
    unordered_map<string, int> counter;
    int total_cnt = 0;

    for (const auto& breed : new_dogs) {
        counter[breed]++;
    }

    for (auto& [breed, num] : shelter) {
        int free_space_for_breed = max_amount.at(breed) - shelter[breed];
        if (free_space_for_breed >= counter[breed]) {
            shelter[breed] += counter[breed];
            total_cnt += counter[breed];
        } else if (free_space_for_breed > 0) {
            shelter[breed] += free_space_for_breed;
            total_cnt += free_space_for_breed;
        } else {
            // Do nothing since the shelter can no longer accepts any dogs
            // cout << "The shelter can no longer accepts any dogs since there are no free space."s << endl;
        }
    }

    return total_cnt;
}

int main() {
    const vector<string> new_dogs = {"landseer"s, "otterhound"s, "otterhound"s, "otterhound"s, "pointer"s};
    const map<string, int> max_amount = {{"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 4}, {"pointer"s, 7}};
    map<string, int> shelter = {{"landseer"s, 1}, {"otterhound"s, 2}, {"pekingese"s, 2}, {"pointer"s, 3}};

    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl;

    return 0;
}