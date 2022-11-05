#include <iostream>
#include <map>
#include <string>

struct Vector3D {
    double x;
    double y;
    double z;
};

int main() {
    using namespace std;

    map<string, string>* string_to_string;

    cout << "char*: size:"s << sizeof(char*) << endl;
    cout << "int*: size:"s << sizeof(int*) << endl;
    cout << "double*: size:"s << sizeof(double*) << endl;
    cout << "Vector3D*: size:"s << sizeof(Vector3D*) << endl;
    cout << "map<string, string>*: size:"s << sizeof(string_to_string) << endl;
}