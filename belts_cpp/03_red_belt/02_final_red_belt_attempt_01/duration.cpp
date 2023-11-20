#include "duration.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

TotalDuration::TotalDuration(const string& msg) : message(msg + ": "), dur(0) {}

TotalDuration::~TotalDuration() {
    ostringstream os;
    os << std::setw(30) << std::left << message << duration_cast<microseconds>(dur).count() << " microseconds"
       << endl;
    cerr << os.str();
}

AddDuration::AddDuration(steady_clock::duration& dest)
    : add_to(dest), start(steady_clock::now()) {}

AddDuration::AddDuration(TotalDuration& dest) : AddDuration(dest.dur) {}

AddDuration::~AddDuration() { add_to += steady_clock::now() - start; }
