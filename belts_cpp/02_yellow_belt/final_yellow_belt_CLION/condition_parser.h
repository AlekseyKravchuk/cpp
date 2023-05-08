#pragma once

#include <iostream>
#include <memory>

#include "node.h"

std::shared_ptr<Node> ParseCondition(std::istream& is);

std::string ParseEvent(std::istream& iss);
