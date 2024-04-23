#include <iostream>
#include <vector>
#include <numeric>   // std::accumulate
#include <utility>   // std::move
#include <string>    // std::to_string
#include <iterator>  // std::next

using namespace std;

void sequential_accumulate_test() {
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int initial_value = 0;
    int sum = std::accumulate(v.begin(), v.end(), initial_value);
    cout << "sum = " << sum << endl;

    // ======= multiplying all the elements of the vector =======
    int initial_product = 1;
    int product = std::accumulate(v.begin(), v.end(),
                                  initial_product,
                                  [](const int lhs, const int rhs) {
                                      return lhs * rhs;
                                  });
    cout << "product = " << product << endl;

    // ======= print all the elements of the vector dashed =======
    // we need 2 arguments for lambda expression: first one for already accumulated string, and second one - for the
    // integer from the vector
    auto dash_fold = [](string accumulated_str, int elm_of_vector) {
        return std::move(accumulated_str) + '-' + std::to_string(elm_of_vector);
    };

    string s = std::accumulate(std::next(v.begin()), v.end(), std::to_string(*v.begin()),
                               dash_fold);
    cout << "dashed elements = " << s << endl;
}

int main() {
    sequential_accumulate_test();

    return 0;
}
