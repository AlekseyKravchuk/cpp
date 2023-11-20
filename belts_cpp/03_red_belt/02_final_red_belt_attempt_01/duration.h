#pragma once

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

struct TotalDuration {
    explicit TotalDuration(const string& msg = "");
    ~TotalDuration();

    std::string message;
    steady_clock::duration dur;
};

class AddDuration {
   public:
    explicit AddDuration(steady_clock::duration& dest);
    explicit AddDuration(TotalDuration& dest);
    ~AddDuration();

   private:
    steady_clock::duration& add_to;
    steady_clock::time_point start;
};

#define TOKEN_PASTE(x, y) _x##y
#define CAT(x, y) TOKEN_PASTE(x, y)
#define UNIQ_ID CAT(id_, __LINE__)

#define ADD_DURATION(value) \
    AddDuration UNIQ_ID{value}
