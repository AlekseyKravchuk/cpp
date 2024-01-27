#include <iostream>
using namespace std;

int main() {
    // An ordinary string
    string str1 = "Welcome.\nTo.\nEducative.\n";

    // Raw string
    string str2 = R"(Welcome.\nTo.\nEducative.\n)";

    cout << "Normal string is: " << endl
         << str1 << endl;

    cout << "Raw string is: " << str2 << endl;

    return 0;
}