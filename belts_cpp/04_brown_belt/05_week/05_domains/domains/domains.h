#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <utility>

std::vector<std::string> ReadDomainsIntoVector(std::istream& in = std::cin);
std::unordered_set<std::string> ReadDomainsInUnorderedSet(std::istream& in = std::cin);
std::vector<std::string> GetDomainStatuses(const std::vector<std::string>& domains_to_check,
                                           const std::unordered_set<std::string>& banned_domains);