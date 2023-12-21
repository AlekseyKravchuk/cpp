#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "spending.h"

std::vector<Spending> LoadFromXml(std::istream& input);

void IgnoreBlankLines(std::istream& input);

std::map<std::string, std::vector<Spending>> LoadFromXmlByMonths(std::istream& input);

std::vector<Spending> LoadFromJson(std::istream& input);
