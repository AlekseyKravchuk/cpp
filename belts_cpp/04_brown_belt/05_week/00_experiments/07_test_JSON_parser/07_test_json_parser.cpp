#include <iostream>
#include <fstream>

#include "json.h"

using namespace std;

int main() {
    std::ifstream in{"input.json"};
    Json::Document document = Json::Load(in);

    cout << "Ok" << endl;

    return 0;
}
