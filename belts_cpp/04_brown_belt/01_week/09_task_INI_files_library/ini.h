#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility> // std::move, std::pair

namespace Ini {

// секция - это хеш-таблица, содержащая набор пар вида <имя_переменной, её_значение> типа <string, string>
using Section = std::unordered_map<std::string, std::string>;

class Document {
  public:
    Section& AddSection(std::string name);
    const Section& GetSection(const std::string& name) const;
    size_t SectionCount() const;

  private:
    // документ - это хеш-таблица, содержащая набор пар вида <имя_секции, содержимое_секции> типа <string, Section>
    std::unordered_map<std::string, Section> _sections;
};

Document Load(std::istream& input);
} // end namespace Ini