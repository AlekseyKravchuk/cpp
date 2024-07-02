#include <iostream>
#include <string>

using namespace std;

void PrintDiagnosticInformation() {
    cout << "Function name: "s << __FUNCTION__ << endl;
    cout << "File name: "s << __FILE__ << endl;
    cout << "Line number: "s << __LINE__ << endl;
    cout << "Line number: "s << __LINE__ << endl;
}

int main() {
    PrintDiagnosticInformation();

    return 0;
}
