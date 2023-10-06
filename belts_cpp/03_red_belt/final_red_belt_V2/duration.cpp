#include "duration.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

TotalDuration::TotalDuration(const string& msg)
    : message(msg + ": "),
      value(0) {}

TotalDuration::~TotalDuration() {
    ostringstream os;
    os << message
       << duration_cast<milliseconds>(value).count()
       << " ms" << endl;
    cerr << os.str();
}

AddDuration::AddDuration(steady_clock::duration& dest)
    : add_to(dest),
      start(steady_clock::now()) {}

AddDuration::AddDuration(TotalDuration& dest)
    : AddDuration(dest.value) {
}

AddDuration::~AddDuration() {
    add_to += steady_clock::now() - start;
}

#define ADD_DURATION(value) \
    AddDuration UNIQ_ID(__LINE__){value};
