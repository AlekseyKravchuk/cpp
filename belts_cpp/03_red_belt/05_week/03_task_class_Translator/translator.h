#pragma once

#include <map>
#include <string_view>
#include <vector>
#include <set>

class Translator {
   public:
    void Add(std::string_view source, std::string_view target);
    std::string_view TranslateForward(std::string_view source) const;
    std::string_view TranslateBackward(std::string_view target) const;

   private:
   std::set<std::string> _sources;
   std::set<std::string> _targets;
   std::map<std::string_view, std::vector<std::string_view>> _forward;
   std::map<std::string_view, std::vector<std::string_view>> _backward;
};