#pragma once

#ifndef _CONDITION_PARSE_H_
#define _CONDITION_PARSE_H_

#include <iostream>
#include <memory>

#include "node.h"
#include "test_runner.h"

//----------------------------------------------------------------------------------------------------
std::shared_ptr<Node> ParseCondition(std::istream& is);
//----------------------------------------------------------------------------------------------------
void TestParseCondition();
//----------------------------------------------------------------------------------------------------
#endif  // _CONDITION_PARSE_H_