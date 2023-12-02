#pragma once

#include <map>
#include <string>

using namespace std::literals;

enum class OPCODE {
    ADD,
    COUNT,
    CHECK
};

std::map<std::string, OPCODE> str2opcode{
    {"ADD"s, OPCODE::ADD},
    {"COUNT"s, OPCODE::COUNT},
    {"CHECK"s, OPCODE::CHECK}};