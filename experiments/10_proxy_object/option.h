//
// Created by kav on 22.01.25.
//
#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Option {
  public:
    std::string _name;
    std::string _description;

    Option(std::string  name, std::string  description)
            : _name(std::move(name)),
              _description(std::move(description)) {}
};
// ================================================

class OptionsDescription {
  public:
    class Proxy {  // Proxy object
      public:
        explicit Proxy(OptionsDescription& opt_description) : _opt_description(opt_description) {}
        Proxy& operator()(const std::string& name, const std::string& description);

      private:
        OptionsDescription& _opt_description;
    };

    void add_option(const std::string& name, const std::string& description);
    Proxy add_options();
    void print() const;

  private:
    std::vector<Option> _options;
};



