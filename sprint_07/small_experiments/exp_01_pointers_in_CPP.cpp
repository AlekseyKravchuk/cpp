#include <iostream>
#include <map>
#include <string>

struct Vector3D {
    double x;
    double y;
    double z;

    Vector3D(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
};

int main() {
    using namespace std;

    Vector3D* vectorPtr = new Vector3D(3.0, 5.2, 8.1);
    std::cout << (*vectorPtr).x << std::endl;
    std::cout << vectorPtr->x << std::endl;

    // int answer = 42;
    // int& answer_ref = answer;

    // int* answerPtr = &answer_ref;

    // std::cout << answerPtr << std::endl;

    
    
    return 0;
}
