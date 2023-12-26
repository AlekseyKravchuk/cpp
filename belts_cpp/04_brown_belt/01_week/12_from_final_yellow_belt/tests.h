#pragma once

#include <sstream>
#include <memory>  // std::shared_ptr
#include <stdexcept>

#include "date.h"
#include "database.h"
#include "condition_parser.h"

std::string Entry(const Date &date, const std::string &event);

auto get_predicate(const std::string& s);

void TestParseCondition();

void TestParseEvent();

void TestAll();