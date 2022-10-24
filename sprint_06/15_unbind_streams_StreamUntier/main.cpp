#include <iostream>

#include "log_duration.h"

using namespace std;

class StreamUntier {
   public:
    StreamUntier(istream& inputStream) : _tiedBeforePtr(inputStream.tie(nullptr)), _istreamRef(inputStream) {}

    ~StreamUntier() {
        _istreamRef.tie(_tiedBeforePtr);
    }

   private:
    ostream* _tiedBeforePtr;  // указатель на объект std::cout, который изначально был привязан к std::cin
    istream& _istreamRef;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(std::cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}