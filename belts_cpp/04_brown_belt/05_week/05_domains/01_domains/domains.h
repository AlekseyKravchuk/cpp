#pragma once

#include <iostream>
#include <vector>
#include <string>

bool IsSubdomainReversed(const std::string& subdomain, const std::string& domain);
std::vector<std::string> ReadDomains(std::istream& in = std::cin);
bool IsBannedDomain(const std::string& domain, const std::vector<std::string>& banned_domains);
