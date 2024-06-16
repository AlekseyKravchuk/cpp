#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

using namespace std;

class Node : std::variant<std::vector<Node>,
                          std::map<std::string, Node>,
                          int,
                          double,
                          std::string> {
  public:
    using variant::variant;

    const auto& AsArray() const {
        return std::get<std::vector<Node>>(*this);
    }

    const auto& AsMap() const {
        return std::get<std::map<std::string, Node>>(*this);
    }

    int AsInt() const {
        return std::get<int>(*this);
    }

    // TODO: implement method
    double AsDouble() const {
        return std::get<double>(*this);
    }

    const auto& AsString() const {
        return std::get<std::string>(*this);
    }
};

Node LoadNumber(istream& input) {
    input >> ws;
    int integer_part = 0;
    bool is_negative = false;

    if (input.peek() == '-') {
        input.ignore(1);
        is_negative = true;
    }

    while (isdigit(input.peek())) {
        integer_part *= 10;
        integer_part += input.get() - '0';
    }

    if (input.peek() == '.') {
        input.ignore(1);
    } else {
        return Node{integer_part};
    }

    int64_t fraction_part = 0;
    while (isdigit(input.peek())) {
        fraction_part *= 10;
        fraction_part += input.get() - '0';
    }

    return is_negative
               ? Node{stod('-' + to_string(integer_part) + '.' + to_string(fraction_part))}
               : Node{stod(to_string(integer_part) + '.' + to_string(fraction_part))};
}

int main() {
    istringstream iss1(R"(345.9754)");
    Node nd1 = LoadNumber(iss1);

    istringstream iss2(R"(0.12345)");
    Node nd2 = LoadNumber(iss2);

    istringstream iss3(R"(-120.964345)");
    Node nd3 = LoadNumber(iss3);

    istringstream iss4(R"(677)");
    Node nd4 = LoadNumber(iss4);

    cout << setprecision(7) << nd1.AsDouble() << endl;
    cout << setprecision(7) << nd2.AsDouble() << endl;
    cout << setprecision(9) << nd3.AsDouble() << endl;
    cout << setprecision(9) << nd4.AsInt() << endl;

    return 0;
}
